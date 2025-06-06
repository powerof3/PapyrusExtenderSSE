#include "Papyrus/Util/ActorGraphics.h"

namespace GRAPHICS
{
	namespace TEXTURE
	{
		void sanitize_path(std::string& a_path)
		{
			std::ranges::transform(a_path, a_path.begin(),
				[](char c) { return static_cast<char>(std::tolower(c)); });

			a_path = srell::regex_replace(a_path, srell::regex("/+|\\\\+"), "\\");
			a_path = srell::regex_replace(a_path, srell::regex("^\\\\+"), "");
			a_path = srell::regex_replace(a_path, srell::regex(R"(.*?[^\s]textures\\|^textures\\)", srell::regex::icase), "");
		}

		RE::BSShaderTextureSet* create_textureset(char** a_value)
		{
			const auto textureset = RE::BSShaderTextureSet::Create();
			if (textureset) {
				for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
					if (!string::is_empty(a_value[type])) {
						textureset->SetTexturePath(type, a_value[type]);
					}
				}
			}
			return textureset;
		}
	}

	ShaderData::Input::Input(RE::TESObjectREFR* a_ref, const RE::BSFixedString& a_filter, std::uint32_t a_shaderType, std::int32_t a_textureType, bool a_noWeapons, bool a_noAlpha) :
		noWeapons(a_noWeapons),
		noAlphaMeshes(a_noAlpha),
		isActor(a_ref->Is(RE::FormType::ActorCharacter)),
		textureType(a_textureType),
		feature(static_cast<Feature>(a_shaderType)),
		sourcePath(a_filter)
	{
		TEXTURE::sanitize_path(sourcePath);
	}

	void ShaderData::Input::SetShaderType(RE::NiAVObject* a_root, RE::NiAVObject* a_templateRoot) const
	{
		if (const auto template_geo = a_templateRoot->GetFirstGeometryOfShaderType(feature.get()); template_geo) {
			std::vector<RE::BSFixedString> results{};
			SetShaderTypeImpl(a_root, template_geo, results);
			if (!results.empty()) {
				const auto name = std::string(EXTRA::SHADER) + std::to_string(feature.underlying());
				EXTRA::Add<RE::NiStringsExtraData>(a_root, name, results);
			}
		}
	}

	void ShaderData::Input::SetShaderTypeImpl(RE::NiAVObject* a_object, RE::BSGeometry* a_templateGeo, std::vector<RE::BSFixedString>& results) const
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			if (!a_geometry->vertexDesc.HasFlag(VertexFlags::VF_NORMAL)) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}
			if (const auto parent = a_geometry->parent; parent && parent->AsFadeNode() && noWeapons && isActor) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}
			if (const auto alpha = a_geometry->properties[States::kProperty]; alpha && noAlphaMeshes) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}

			const auto& effect = a_geometry->properties[States::kEffect];
			const auto  lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

			const auto& tempEffect = a_templateGeo->properties[States::kEffect];
			const auto  tempLightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(tempEffect.get());

			if (lightingShader && tempLightingShader) {
				const auto material = static_cast<MaterialBase*>(lightingShader->material);
				const auto tempMaterial = static_cast<MaterialBase*>(tempLightingShader->material);

				if (material && tempMaterial) {
					const auto& textureSet = material->textureSet;
					const auto& tempTextureSet = tempMaterial->textureSet;

					if (textureSet && tempTextureSet) {
						std::string sourceDiffuse{ textureSet->GetTexturePath(Texture::kDiffuse) };
						TEXTURE::sanitize_path(sourceDiffuse);
						if (!sourcePath.empty() && !sourceDiffuse.contains(sourcePath)) {
							return RE::BSVisit::BSVisitControl::kContinue;
						}

						const auto oldFeature = material->GetFeature();
						const auto oldFlags = lightingShader->flags.get();
						const auto oldEmissiveColor = lightingShader->emissiveColor ? lightingShader->emissiveColor->ToHex() : "000000";
						const auto oldEmissiveMult = lightingShader->emissiveMult;

						const auto newFeature = tempMaterial->GetFeature();

						if (oldFeature != newFeature) {
							if (const auto data = lightingShader->GetExtraData<RE::NiStringsExtraData>(EXTRA::ORIG_SHADER); !data) {
								std::vector<RE::BSFixedString> ActorResetter;
								ActorResetter.resize(TYPE::kShaderTotal);

								for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
									ActorResetter[type] = textureSet->GetTexturePath(type);  //0-8
								}
								ActorResetter[TYPE::kFeature] = std::to_string(std::to_underlying(oldFeature));  //9
								ActorResetter[TYPE::kFlag] = std::to_string(std::to_underlying(oldFlags));       //10
								ActorResetter[TYPE::kColor] = oldEmissiveColor;                                  //11
								ActorResetter[TYPE::kColorMult] = std::to_string(oldEmissiveMult);               //12

								const auto newData = RE::NiStringsExtraData::Create(EXTRA::ORIG_SHADER, ActorResetter);
								if (newData && lightingShader->AddExtraData(newData)) {
									results.push_back(a_geometry->name);
								}
							}

							if (const auto newMaterial = static_cast<MaterialBase*>(tempMaterial->Create()); newMaterial) {
								newMaterial->CopyMembers(tempMaterial);
								newMaterial->ClearTextures();

								if (textureType != -1) {
									const auto BSTextureType = static_cast<Texture>(textureType);

									if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
										for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
											if (type != BSTextureType) {
												newTextureSet->SetTexturePath(type, tempTextureSet->GetTexturePath(type));
											}
										}
										newTextureSet->SetTexturePath(BSTextureType, textureSet->GetTexturePath(BSTextureType));
										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								}

								lightingShader->CopyMembers(tempLightingShader);
								lightingShader->SetFlags(ShaderFlags8::kSkinned, a_geometry->vertexDesc.HasFlag(VertexFlags::VF_SKINNED));

								lightingShader->SetMaterial(newMaterial, true);
								lightingShader->SetupGeometry(a_geometry);
								lightingShader->FinishSetupGeometry(a_geometry);

								newMaterial->~BSLightingShaderMaterialBase();
								RE::free(newMaterial);
							}
						}
					}
				}
			}

			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}

	ShaderData::Output::Output(RE::NiStringsExtraData* a_data)
	{
		hasData = false;

		if (a_data && a_data->value && a_data->size == kShaderTotal) {
			if (const auto new_txst = TEXTURE::create_textureset(a_data->value); new_txst) {
				textureSet = new_txst;
				feature = string::to_num<Feature>(a_data->value[kFeature]);
				flags = string::to_num<std::uint64_t>(a_data->value[kFlag]);
				emissiveColor = RE::NiColor(string::to_num<std::uint32_t>(a_data->value[kColor]));
				emissiveMult = string::to_num<float>(a_data->value[kColorMult]);
				hasData = true;
			}
		}
	}

	bool ShaderData::Output::Reset(RE::BSGeometry* a_geometry, RE::BSLightingShaderProperty* a_shaderProp, MaterialBase* a_material) const
	{
		if (!hasData) {
			return false;
		}

		if (const auto newMaterial = MaterialBase::CreateMaterial(feature.get()); newMaterial) {
			newMaterial->CopyBaseMembers(a_material);
			newMaterial->ClearTextures();
			newMaterial->OnLoadTextureSet(0, textureSet);

			a_shaderProp->flags = static_cast<RE::BSShaderProperty::EShaderPropertyFlag>(flags);
			a_shaderProp->lastRenderPassState = std::numeric_limits<std::int32_t>::max();
			if (a_shaderProp->flags.all(ShaderFlags::kOwnEmit)) {
				if (!a_shaderProp->emissiveColor) {
					a_shaderProp->emissiveColor = new RE::NiColor();
				}
				*a_shaderProp->emissiveColor = emissiveColor;
			}
			a_shaderProp->emissiveMult = emissiveMult;

			a_shaderProp->SetMaterial(newMaterial, true);
			a_shaderProp->SetupGeometry(a_geometry);
			a_shaderProp->FinishSetupGeometry(a_geometry);

			newMaterial->~BSLightingShaderMaterialBase();
			RE::free(newMaterial);
		}

		return true;
	}

	void ActorApplier::ToggleNode(RE::NiAVObject* a_root, RE::NiAVObject* a_node, bool a_cull)
	{
		a_node->CullNode(a_cull);

		if (const auto data = a_root->GetExtraData<RE::NiStringsExtraData>(EXTRA::TOGGLE); data) {
			a_cull ?
				data->Insert(a_node->name) :
				data->Remove(a_node->name);
		} else if (a_cull) {
			std::vector<RE::BSFixedString> vec{ a_node->name };
			if (const auto newData = RE::NiStringsExtraData::Create(EXTRA::TOGGLE, vec); newData) {
				a_root->AddExtraData(newData);
			}
		}
	}

	void ActorApplier::BodyTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiColor color)
	{
		tint_face(a_actor, color);
		a_root->UpdateBodyTint(color);
		update_color_data(a_root, EXTRA::SKIN_TINT, color);
	}

	void ActorApplier::HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiColor color)
	{
		a_root->UpdateHairColor(color);
		if (const auto& biped = a_actor->GetCurrentBiped(); biped) {
			for (auto& slot : headSlots) {
				const auto& node = biped->objects[slot].partClone;
				if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
					node->UpdateHairColor(color);
				}
			}
		}
		update_color_data(a_root, EXTRA::HAIR_TINT, color);
	}

	void ActorApplier::ArmorTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			const auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
			const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

			const auto material = lightingShader ? static_cast<MaterialBase*>(lightingShader->material) : nullptr;
			const auto textureSet = material ? material->textureSet : RE::NiPointer<RE::BSTextureSet>();

			if (lightingShader && material) {
				if (textureSet) {
					std::string sourcePath{ textureSet->GetTexturePath(Texture::kDiffuse) };
					TEXTURE::sanitize_path(sourcePath);

					if (sourcePath == a_tgtPath) {
						if (const auto newMaterial = static_cast<MaterialBase*>(material->Create()); newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();

							if (a_type == -1) {
								newMaterial->OnLoadTextureSet(0, a_txst);
							} else {
								if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
									const auto BSTextureType = static_cast<Texture>(a_type);

									for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
										if (type != BSTextureType) {
											newTextureSet->SetTexturePath(type, textureSet->GetTexturePath(type));
										}
									}

									newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							}

							lightingShader->SetMaterial(newMaterial, true);

							lightingShader->SetupGeometry(a_geometry);
							lightingShader->FinishSetupGeometry(a_geometry);

							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);

							replaced = true;
						}
					}
				}
			}

			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}

	void ActorApplier::SkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			const auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
			const auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

			const auto material = lightingShader ? static_cast<MaterialBase*>(lightingShader->material) : nullptr;

			if (lightingShader && material) {
				auto const feature = material->GetFeature();
				if (const auto textureSet = material->textureSet; textureSet && stl::is_in(feature, Feature::kFaceGenRGBTint, Feature::kFaceGen)) {
					if (a_vec.empty()) {
						a_vec.resize(Texture::kTotal);
						for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
							a_vec[type] = textureSet->GetTexturePath(type);
						}
					}
					if (const auto newMaterial = static_cast<MaterialBase*>(material->Create()); newMaterial) {
						newMaterial->CopyMembers(material);
						newMaterial->ClearTextures();

						if (a_type == -1) {
							if (feature == Feature::kFaceGen) {
								if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
									for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
										if (type != Texture::kMultilayer) {
											newTextureSet->SetTexturePath(type, a_txst->GetTexturePath(type));
										}
									}
									newTextureSet->SetTexturePath(Texture::kMultilayer, textureSet->GetTexturePath(Texture::kMultilayer));
									newMaterial->OnLoadTextureSet(0, newTextureSet);
								}
							} else {
								newMaterial->OnLoadTextureSet(0, a_txst);
							}
						} else {
							if (const auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
								const auto BSTextureType = static_cast<Texture>(a_type);
								for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
									if (type != BSTextureType) {
										newTextureSet->SetTexturePath(type, material->textureSet->GetTexturePath(type));
									}
								}
								newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
								newMaterial->OnLoadTextureSet(0, newTextureSet);
							}
						}

						lightingShader->SetMaterial(newMaterial, true);
						lightingShader->SetupGeometry(a_geometry);
						lightingShader->FinishSetupGeometry(a_geometry);

						newMaterial->~BSLightingShaderMaterialBase();
						RE::free(newMaterial);
					}
				}
			}

			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}

	void ActorApplier::ArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BIPED_MODEL::BipedObjectSlot a_slot, std::int32_t a_type)
	{
		const auto skinArmor = a_actor->GetSkin(a_slot);
		if (!skinArmor) {
			return;
		}

		const auto skinArma = skinArmor->GetArmorAddonByMask(a_actor->GetRace(), a_slot);
		if (!skinArma) {
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, a_txst, a_slot, a_type, skinArmor, skinArma]() {
			std::vector<RE::BSFixedString> results;
			a_actor->VisitArmorAddon(skinArmor, skinArma, [&](bool, RE::NiAVObject& a_obj) -> bool {
				ActorApplier::SkinTXST(&a_obj, a_txst, results, a_type);
				return true;
			});
			const auto root = a_actor->Get3D(false);
			if (!results.empty() && root) {
				auto       slotMaskStr = std::to_string(std::to_underlying(a_slot));
				const auto name = "PO3_SKINTXST - " + slotMaskStr;
				results.emplace_back(slotMaskStr);

				EXTRA::Add<RE::NiStringsExtraData>(root, name, results);
			}
		});
	}

	void ActorApplier::tint_face(RE::Actor* a_actor, const RE::NiColor& a_color)
	{
		const auto object = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace);
		const auto geometry = object ? object->AsGeometry() : nullptr;

		if (geometry) {
			const auto& effect = geometry->properties[States::kEffect];
			const auto  lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
			if (lightingShader) {
				const auto material = static_cast<MaterialBase*>(lightingShader->material);
				if (material && material->GetFeature() == Feature::kFaceGen) {
					const auto facegenTint = MaterialBase::CreateMaterial<RE::BSLightingShaderMaterialFacegenTint>();
					if (facegenTint) {
						facegenTint->CopyBaseMembers(material);

						lightingShader->SetFlags(ShaderFlags8::kFace, false);
						lightingShader->SetFlags(ShaderFlags8::kFaceGenRGBTint, true);

						lightingShader->SetMaterial(facegenTint, true);
						lightingShader->SetupGeometry(geometry);
						lightingShader->FinishSetupGeometry(geometry);

						facegenTint->~BSLightingShaderMaterialFacegenTint();
						RE::free(facegenTint);
					}
				}
			}

			geometry->UpdateBodyTint(a_color);
		}
	}

	void ActorApplier::update_color_data(RE::NiAVObject* a_root, const RE::BSFixedString& a_name, const RE::NiColor& a_color)
	{
		if (const auto data = a_root->GetExtraData<RE::NiIntegerExtraData>(a_name); data) {
			if (a_color != RE::NiColor(data->value)) {
				data->value = a_color.ToInt();
			}
		} else {
			if (const auto newData = RE::NiIntegerExtraData::Create(a_name, a_color.ToInt()); newData) {
				a_root->AddExtraData(newData);
			}
		}
	}

	ActorResetter::ActorResetter(RE::Actor* a_actor, RE::NiAVObject* a_object, const RE::BSFixedString& a_folderName) :
		actor(a_actor),
		root(a_object),
		folderName(a_folderName)
	{
		if (!a_object->extra || a_object->extraDataSize == 0) {
			hasData = false;
			return;
		}

		std::span<RE::NiExtraData*> span(a_object->extra, a_object->extraDataSize);
		for (const auto& extraData : span) {
			if (!extraData) {
				continue;
			}
			if (const auto name = extraData->GetName(); !name.empty()) {
				switch (string::const_hash(name)) {
				case string::const_hash(EXTRA::TOGGLE):
					{
						toggle = static_cast<RE::NiStringsExtraData*>(extraData);
						hasData = true;
					}
					break;
				case string::const_hash(EXTRA::SKIN_TINT):
					{
						tintSkin = static_cast<RE::NiIntegerExtraData*>(extraData);
						hasData = true;
					}
					break;
				case string::const_hash(EXTRA::HAIR_TINT):
					{
						tintHair = static_cast<RE::NiIntegerExtraData*>(extraData);
						hasData = true;
					}
					break;
				case string::const_hash(EXTRA::SKIN_ALPHA):
					{
						alphaSkin = static_cast<RE::NiBooleanExtraData*>(extraData);
						hasData = true;
					}
					break;
				case string::const_hash(EXTRA::FACE_TXST):
					{
						txstFace = static_cast<RE::NiStringsExtraData*>(extraData);
						hasData = true;
					}
					break;
				default:
					if (string::icontains(name, EXTRA::HEADPART)) {
						alphaHDPT.emplace_back(static_cast<RE::NiIntegerExtraData*>(extraData));
						hasData = true;

					} else if (string::icontains(name, EXTRA::TXST)) {
						txst.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
						hasData = true;

					} else if (string::icontains(name, EXTRA::SKIN_TXST)) {
						txstSkin.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
						hasData = true;

					} else if (string::icontains(name, EXTRA::SHADER)) {
						shaders.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
						hasData = true;
					}
					break;
				}
			}
		}

		if (!folderName.empty()) {
			TEXTURE::sanitize_path(folderName);
		}
	}

	bool ActorResetter::ResetActor3D() const
	{
		if (!hasData) {
			return false;
		}

		SKSE::GetTaskInterface()->AddTask([this]() {
			ResetToggle();
			ResetSkinAlpha();
			ResetHeadPartAlpha();
			ResetSkinTint();
			ResetHairTint();
			ResetFaceTXST();
			ResetSkinTXST();
			ResetArmorTXST();
			ResetMaterialShader();
		});

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			if (!actor->IsPlayerRef()) {
				processLists->StopAllMagicEffects(*actor);
			} else {
				stop_all_skin_shaders(actor);
			}
		}

		return true;
	}

	void ActorResetter::ResetToggle() const
	{
		if (toggle && toggle->value && toggle->size > 0) {
			std::span<char*> span(toggle->value, toggle->size);
			for (const auto& string : span) {
				if (!string::is_empty(string)) {
					if (const auto object = root->GetObjectByName(string); object) {
						object->CullNode(false);
					}
				}
			}
			root->RemoveExtraData(toggle->GetName());
		}
	}

	void ActorResetter::ResetSkinTint() const
	{
		if (tintSkin) {
			const auto actorbase = actor->GetActorBase();
			const auto facePart = actorbase ? actorbase->GetCurrentHeadPartByType(HeadPartType::kFace) : nullptr;
			const auto faceNode = actor->GetFaceNodeSkinned();

			if (faceNode && facePart) {
				if (const auto faceGen = RE::BSFaceGenManager::GetSingleton(); faceGen) {
					faceGen->PrepareHeadPartForShaders(faceNode, facePart, actorbase);
				}
				root->UpdateBodyTint(actorbase->bodyTintColor);
				root->RemoveExtraData(tintSkin->GetName());
			}
		}
	}

	void ActorResetter::ResetHairTint() const
	{
		if (tintHair) {
			const auto actorBase = actor->GetActorBase();
			const auto headData = actorBase ? actorBase->headRelatedData : nullptr;
			const auto colorForm = headData ? headData->hairColor : nullptr;

			if (colorForm) {
				root->UpdateHairColor(colorForm->color);

				if (const auto& biped = actor->GetCurrentBiped(); biped) {
					for (auto& slot : headSlots) {
						const auto& node = biped->objects[slot].partClone;
						if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
							node->UpdateHairColor(colorForm->color);
						}
					}
				}

				root->RemoveExtraData(tintHair->GetName());
			}
		}
	}

	void ActorResetter::ResetSkinAlpha() const
	{
		if (alphaSkin) {
			root->UpdateMaterialAlpha(1.0f, true);
			root->RemoveExtraData(alphaSkin->GetName());
		}
	}

	void ActorResetter::ResetHeadPartAlpha() const
	{
		for (auto& data : alphaHDPT) {
			if (data) {
				if (const auto object = actor->GetHeadPartObject(static_cast<HeadPartType>(data->value)); object) {
					object->UpdateMaterialAlpha(1.0f, false);
					root->RemoveExtraData(data->GetName());
				}
			}
		}
	}

	void ActorResetter::ResetFaceTXST() const
	{
		if (txstFace && txstFace->value) {
			const auto textureset = TEXTURE::create_textureset(txstFace->value);
			const auto faceObject = actor->GetHeadPartObject(HeadPartType::kFace);
			if (textureset && faceObject) {
				reset_textureset(faceObject, textureset, true);
				root->RemoveExtraData(txstFace->GetName());
			}
		}
	}

	void ActorResetter::ResetArmorTXST() const
	{
		if (txst.empty() || folderName.empty()) {
			return;
		}

		for (auto& data : txst) {
			if (data && data->value && data->size > 0) {
				RE::FormID formID = 0;
				if (std::string armorID{ data->value[data->size - 1] }; !armorID.empty()) {
					try {
						formID = string::to_num<RE::FormID>(armorID, true);
					} catch (...) {
						continue;
					}
				}
				auto armor = actor->GetWornArmor(formID);
				if (!armor) {
					armor = actor->GetSkin();
				}
				if (armor) {
					const auto textureset = TEXTURE::create_textureset(data->value);
					const auto arma = armor->GetArmorAddon(actor->GetRace());
					if (textureset && arma) {
						actor->VisitArmorAddon(armor, arma, [&](bool, RE::NiAVObject& a_obj) -> bool {
							reset_textureset(&a_obj, textureset, false, folderName);
							return true;
						});
						root->RemoveExtraData(data->GetName());
					}
				}
			}
		}
	}

	void ActorResetter::ResetSkinTXST() const
	{
		if (txstSkin.empty()) {
			return;
		}

		for (auto& data : txstSkin) {
			if (data && data->value && data->size > 0) {
				auto slot = Slot::kNone;
				if (std::string slotMaskstr{ data->value[data->size - 1] }; !slotMaskstr.empty()) {
					try {
						slot = string::to_num<Slot>(slotMaskstr);
					} catch (...) {
						continue;
					}
				}
				if (const auto skinArmor = actor->GetSkin(slot); skinArmor) {
					const auto textureset = TEXTURE::create_textureset(data->value);
					const auto skinArma = skinArmor->GetArmorAddonByMask(actor->GetRace(), slot);
					if (textureset && skinArma) {
						actor->VisitArmorAddon(skinArmor, skinArma, [&](bool, RE::NiAVObject& a_obj) -> bool {
							reset_textureset(&a_obj, textureset, true);
							return true;
						});
						root->RemoveExtraData(data->GetName());
					}
				}
			}
		}
	}

	void ActorResetter::ResetMaterialShader() const
	{
		if (shaders.empty()) {
			return;
		}

		for (auto& data : shaders) {
			if (data && data->value && data->size > 0) {
				std::vector<RE::BSFixedString> vec({ data->value, data->value + data->size });
				reset_shaderdata(root, vec);

				root->RemoveExtraData(data->GetName());
			}
		}
	}

	void ActorResetter::stop_all_skin_shaders(RE::TESObjectREFR* a_ref)
	{
		using Flags = RE::EffectShaderData::Flags;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->ForEachShaderEffect([&](RE::ShaderReferenceEffect* a_shaderEffect) {
				if (a_shaderEffect->target == handle) {
					if (const auto effectData = a_shaderEffect->effectData; effectData &&
																			effectData->data.flags.all(Flags::kSkinOnly) &&
																			!effectData->holesTexture.textureName.empty()) {
						a_shaderEffect->finished = true;
					}
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}
	}

	void ActorResetter::reset_textureset(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_doOnlySkin, const std::string& a_folder)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			const auto& effect = a_geometry->properties[States::kEffect];
			const auto  lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
			if (lightingShader) {
				const auto material = static_cast<MaterialBase*>(lightingShader->material);
				if (material) {
					if (!a_doOnlySkin) {
						if (const auto textureSet = material->textureSet; textureSet && !a_folder.empty()) {
							std::string sourcePath{ textureSet->GetTexturePath(Texture::kDiffuse) };
							if (TEXTURE::sanitize_path(sourcePath); !sourcePath.contains(a_folder)) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}
						}
					} else {
						if (const auto feature = material->GetFeature(); feature != Feature::kFaceGenRGBTint && feature != Feature::kFaceGen) {
							return RE::BSVisit::BSVisitControl::kContinue;
						}
					}

					if (const auto newMaterial = static_cast<MaterialBase*>(material->Create()); newMaterial) {
						newMaterial->CopyMembers(material);

						newMaterial->ClearTextures();
						newMaterial->OnLoadTextureSet(0, a_txst);

						lightingShader->SetMaterial(newMaterial, true);

						lightingShader->SetupGeometry(a_geometry);
						lightingShader->FinishSetupGeometry(a_geometry);

						newMaterial->~BSLightingShaderMaterialBase();
						RE::free(newMaterial);
					}
				}
			}
			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}

	void ActorResetter::reset_shaderdata(RE::NiAVObject* a_object, std::vector<RE::BSFixedString>& a_geometries)
	{
		RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
			if (std::ranges::find(a_geometries, a_geometry->name) == a_geometries.end()) {
				return RE::BSVisit::BSVisitControl::kContinue;
			}

			const auto& effect = a_geometry->properties[States::kEffect];
			const auto  lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

			if (lightingShader) {
				const auto originalData = lightingShader->GetExtraData<RE::NiStringsExtraData>(EXTRA::ORIG_SHADER);
				if (!originalData) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}

				if (const auto material = static_cast<MaterialBase*>(lightingShader->material)) {
					auto shaderData = ShaderData::Output(originalData);
					if (!shaderData.Reset(a_geometry, lightingShader, material)) {
						logger::warn("unable to get original shader values for {}", a_geometry->name.c_str());
					}
					lightingShader->RemoveExtraData(originalData->GetName());
					a_geometry->RemoveExtraData(originalData->GetName());
				}
			}

			return RE::BSVisit::BSVisitControl::kContinue;
		});
	}
}
