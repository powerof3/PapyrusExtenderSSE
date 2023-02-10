#pragma once

namespace Papyrus::Graphics
{
	using namespace GRAPHICS;

	inline void ApplyMaterialShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BGSMaterialObject* a_shader,
		float a_materialThresholdAngle)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (!a_shader) {
			a_vm->TraceStack("Material Object is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		const auto projectedUVParams = RE::NiColorA{
			a_shader->directionalData.falloffScale,
			a_shader->directionalData.falloffBias,
			1.0f / a_shader->directionalData.noiseUVScale,
			std::cosf(RE::deg_to_rad(a_materialThresholdAngle))
		};

		root->SetProjectedUVData(
			projectedUVParams,
			a_shader->directionalData.singlePassColor,
			a_shader->directionalData.flags.any(RE::BSMaterialObject::DIRECTIONAL_DATA::Flag::kSnow));
	}

	//TO_DO
	/*inline bool AttachModel(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::BSFixedString a_path,
		RE::BSFixedString a_nodeName,
		std::vector<float> a_translate,
		std::vector<float> a_rotate,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return false;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID, Severity::kInfo);
			return false;
		}
	}*/

	inline void BlendColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color,
		std::uint32_t a_blendMode,
		bool a_autoCalc,
		float a_opacity)
	{
		using BLEND_MODE = RE::ColorUtil::BLEND_MODE;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("Colorform is None", a_stackID);
			return;
		}

		auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		if (const auto actorbase = a_actor->GetActorBase()) {
			const float opacity = a_autoCalc ?
                                      std::clamp(a_opacity * RE::ColorUtil::CalcLuminance(actorbase->bodyTintColor), 0.0f, 1.0f) :
                                      a_opacity;
			auto newColor = RE::ColorUtil::Blend(actorbase->bodyTintColor, a_color->color, static_cast<BLEND_MODE>(a_blendMode), opacity);

			SKSE::GetTaskInterface()->AddTask([a_actor, newColor, root]() {
				SET::tint_face(a_actor, newColor);

				root->UpdateBodyTint(newColor);

				SET::update_color_data(root, EXTRA::SKIN_TINT, newColor);
			});
		}
	}

	inline bool HasSkin(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::TESObjectARMO* a_check)
	{
		bool result = false;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return result;
		}
		if (!a_check) {
			return result;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return result;
		}

		if (const auto arma = a_check->GetArmorAddon(a_actor->GetRace()); arma) {
			a_actor->VisitArmorAddon(a_check, arma, [&](bool, RE::NiAVObject& a_obj) -> void {
				if (a_obj.HasShaderType(RE::BSShaderMaterial::Feature::kFaceGenRGBTint)) {
					result = true;
				}
			});
		}

		return result;
	}

	inline RE::BGSColorForm* GetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto root = a_actor->Get3D(false);
		const auto data = root ? root->GetExtraData<RE::NiIntegerExtraData>(EXTRA::HAIR_TINT) : nullptr;

		if (data) {
			auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>();
			auto color = factory ? factory->Create() : nullptr;
			if (color) {
				color->color = RE::Color(data->value);
				return color;
			}
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto headData = actorbase ? actorbase->headRelatedData : nullptr;
		return headData ? headData->hairColor : nullptr;
	}

	inline RE::BGSTextureSet* GetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::uint32_t a_type)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto actorBase = a_actor->GetActorBase();
		const auto headpart = actorBase ? actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type)) : nullptr;

		return headpart ? headpart->textureSet : nullptr;
	}

	inline RE::BGSColorForm* GetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return nullptr;
		}

		const auto actorBase = a_actor->GetActorBase();
		const auto factory = actorBase ? RE::IFormFactory::GetConcreteFormFactoryByType<RE::BGSColorForm>() : nullptr;
		const auto color = factory ? factory->Create() : nullptr;

		if (color) {
			const auto root = a_actor->Get3D(false);
			const auto data = root ? root->GetExtraData<RE::NiIntegerExtraData>(EXTRA::SKIN_TINT) : nullptr;
			if (data) {
				color->color = RE::Color(data->value);
			} else {
				color->color = actorBase->bodyTintColor;
			}

			return color;
		}

		return nullptr;
	}

	inline void MixColorWithSkinTone(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color,
		bool a_manual,
		float a_percent)
	{
		a_vm->TraceStack("Function is deprecated, use BlendColorWithSkinTone instead", a_stackID);

		if (a_actor && a_color) {
			const auto actorbase = a_actor->GetActorBase();
			const auto root = a_actor->Get3D(false);
			if (actorbase && root) {
				const float skinLuminance = a_manual ? a_percent : RE::ColorUtil::CalcLuminance(actorbase->bodyTintColor);
				auto newColor = RE::ColorUtil::Mix(actorbase->bodyTintColor, a_color->color, skinLuminance);

				SKSE::GetTaskInterface()->AddTask([a_actor, newColor, root]() {
					SET::tint_face(a_actor, newColor);

					root->UpdateBodyTint(newColor);

					SET::update_color_data(root, EXTRA::SKIN_TINT, newColor);
				});
			}
		}
	}

	inline void PlayDebugShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::vector<float> a_rgba)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		if (a_rgba.size() != 4) {
			a_vm->TraceStack("RGBA array is not valid", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		RE::NiColorA color{ a_rgba[0], a_rgba[1], a_rgba[2], a_rgba[3] };

		SKSE::GetTaskInterface()->AddTask([root, color]() {
			root->TintScenegraph(color);
		});
	}

	inline void ReplaceArmorTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::TESObjectARMO* a_armor,
		RE::BGSTextureSet* a_srcTXST,
		RE::BGSTextureSet* a_tgtTXST,
		std::int32_t a_type)
	{
		using Texture = RE::BSShaderTextureSet::Textures::Texture;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_armor) {
			a_vm->TraceStack("Armor is None", a_stackID);
			return;
		}
		if (!a_srcTXST) {
			a_vm->TraceStack("Source TextureSet is None", a_stackID);
			return;
		}
		if (!a_tgtTXST) {
			a_vm->TraceStack("Target TextureSet is None", a_stackID);
			return;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		std::string targetPath{ a_srcTXST->GetTexturePath(Texture::kDiffuse) };
		TEXTURE::sanitize_path(targetPath);

		SKSE::GetTaskInterface()->AddTask([a_actor, a_armor, a_srcTXST, a_tgtTXST, a_type, targetPath]() {
			bool replaced = false;

			if (const auto arma = a_armor->GetArmorAddon(a_actor->GetRace()); arma) {
				a_actor->VisitArmorAddon(a_armor, arma, [&](bool, RE::NiAVObject& a_obj) -> bool {
					SET::ArmorTXST(&a_obj, a_tgtTXST, a_type, targetPath, replaced);
					return true;
				});
			}

			const auto root = a_actor->Get3D(false);
			if (replaced && root) {
				auto armorID = std::to_string(a_armor->formID);
				const auto name = "PO3_TXST - " + armorID;

				const auto data = root->GetExtraData<RE::NiStringsExtraData>(name);
				if (!data) {
					std::vector<RE::BSFixedString> result;
					result.reserve(Texture::kTotal);
					for (const auto& type : TEXTURE::types) {
						result.emplace_back(a_srcTXST->GetTexturePath(type));
					}
					result.emplace_back(armorID);
					if (const auto newData = RE::NiStringsExtraData::Create(name, result); newData) {
						root->AddExtraData(newData);
					}
				}
			}
		});
	}

	inline void ReplaceFaceTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSTextureSet* a_maleTXST,
		RE::BGSTextureSet* a_femaleTXST,
		std::int32_t a_type)
	{
		using Texture = RE::BSShaderTextureSet::Texture;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		const auto actorBase = a_actor->GetActorBase();
		const bool isFemale = actorBase && actorBase->IsFemale();

		if (const auto txst = isFemale ? a_femaleTXST : a_maleTXST; txst) {
			SKSE::GetTaskInterface()->AddTask([txst, a_type, a_actor]() {
				if (const auto faceObject = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace); faceObject) {
					std::vector<RE::BSFixedString> result;
					SET::SkinTXST(faceObject, txst, result, a_type);

					const auto root = a_actor->Get3D(false);
					if (!result.empty() && root) {
						SET::add_data_if_none<RE::NiStringsExtraData>(root, EXTRA::FACE_TXST, result);
					}
				}
			});
		}
	}

	inline void ReplaceSkinTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSTextureSet* a_maleTXST,
		RE::BGSTextureSet* a_femaleTXST,
		std::uint32_t a_slot,
		std::int32_t a_type)
	{
		using BipedSlot = RE::BIPED_MODEL::BipedObjectSlot;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_actor->Is3DLoaded()) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		const auto actorBase = a_actor->GetActorBase();
		const bool isFemale = actorBase && actorBase->IsFemale();

		if (isFemale && !a_femaleTXST) {
			a_vm->TraceStack("Female TextureSet is None", a_stackID);
			return;
		}
		if (!isFemale && !a_maleTXST) {
			a_vm->TraceStack("Male TextureSet is None", a_stackID);
			return;
		}

		SET::ArmorSkinTXST(a_actor,
			isFemale ? a_femaleTXST : a_maleTXST,
			static_cast<BipedSlot>(a_slot), a_type);
	}

	inline bool ResetActor3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BSFixedString a_folderName)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return false;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return false;
		}

		bool result = true;
		RESET::ResetData resetData;

		std::tie(result, resetData) = RESET::get_data(root);
		if (!result) {
			return false;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, a_folderName, root, resetData]() {
			auto& [toggleData, skinTintData, hairTintData, skinAlphaData, txstFaceData, headpartAlphaVec, txstVec, txstSkinVec, shaderVec] = resetData;

			RESET::Toggle(root, toggleData);
			RESET::SkinAlpha(root, skinAlphaData);
			RESET::HeadPartAlpha(a_actor, root, headpartAlphaVec);
			RESET::SkinTint(a_actor, root, skinTintData);
			RESET::HairTint(a_actor, root, hairTintData);
			RESET::FaceTXST(a_actor, root, txstFaceData);
			RESET::SkinTXST(a_actor, root, txstSkinVec);

			if (!a_folderName.empty()) {
				RESET::ArmorTXST(a_actor, root, a_folderName, txstVec);
			}

			RESET::MaterialShader(root, shaderVec);
		});

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			if (!a_actor->IsPlayerRef()) {
				processLists->StopAllMagicEffects(*a_actor);
			} else {
				RESET::stop_all_skin_shaders(a_actor);
			}
		}

		return true;
	}

	inline void ScaleObject3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::string a_nodeName,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		const auto scale_collision = [&](const RE::NiPointer<RE::bhkWorldObject>& a_body) {
			if (!a_body) {
				return;
			}

			const auto hkpBody = static_cast<RE::hkpWorldObject*>(a_body->referencedObject.get());
			const auto hkpShape = hkpBody ? hkpBody->GetShape() : nullptr;

			if (hkpShape) {
				switch (hkpShape->type) {
				case RE::hkpShapeType::kBox:
					{
						if (const auto boxShape = const_cast<RE::hkpBoxShape*>(static_cast<const RE::hkpBoxShape*>(hkpShape)); boxShape) {
							boxShape->halfExtents = boxShape->halfExtents * RE::hkVector4(a_scale);
						}
					}
					break;
				case RE::hkpShapeType::kSphere:
					{
						if (const auto sphereShape = const_cast<RE::hkpSphereShape*>(static_cast<const RE::hkpSphereShape*>(hkpShape)); sphereShape) {
							sphereShape->radius *= a_scale;
						}
					}
					break;
				case RE::hkpShapeType::kCapsule:
					{
						if (const auto capsuleShape = const_cast<RE::hkpCapsuleShape*>(static_cast<const RE::hkpCapsuleShape*>(hkpShape)); capsuleShape) {
							const float radius = capsuleShape->radius * a_scale;

							capsuleShape->radius = radius;
							capsuleShape->vertexA.quad.m128_f32[3] = radius;
							capsuleShape->vertexB.quad.m128_f32[3] = radius;
						}
					}
					break;
				default:
					break;
				}
			}
		};

		if (!a_nodeName.empty()) {
			if (const auto object = root->GetObjectByName(a_nodeName); object) {
				SKSE::GetTaskInterface()->AddTask([object, a_scale]() {
					object->local.scale *= a_scale;

					RE::NiUpdateData updateData{ 0.0f, RE::NiUpdateData::Flag::kNone };
					object->Update(updateData);
				});

				const auto cell = a_ref->GetParentCell();
				const auto world = cell ? cell->GetbhkWorld() : nullptr;

				if (world) {
					RE::BSWriteLockGuard locker(world->worldLock);

					if (const auto node = object->AsNode(); node) {
						RE::BSVisit::TraverseScenegraphCollision(node, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
							scale_collision(a_col->body);

							return RE::BSVisit::BSVisitControl::kContinue;
						});
					} else {
						if (const auto col = static_cast<RE::bhkNiCollisionObject*>(object->collisionObject.get()); col) {
							scale_collision(col->body);
						}
					}
				}
			}
		} else {
			SKSE::GetTaskInterface()->AddTask([root, a_scale]() {
				root->local.scale *= a_scale;

				RE::NiUpdateData updateData{ 0.0f, RE::NiUpdateData::Flag::kNone };
				root->Update(updateData);
			});

			const auto cell = a_ref->GetParentCell();
			const auto world = cell ? cell->GetbhkWorld() : nullptr;

			if (world) {
				RE::BSWriteLockGuard locker(world->worldLock);

				RE::BSVisit::TraverseScenegraphCollision(root, [&](const RE::bhkNiCollisionObject* a_col) -> RE::BSVisit::BSVisitControl {
					scale_collision(a_col->body);
					return RE::BSVisit::BSVisitControl::kContinue;
				});
			}
		}
	}

	inline void SetHairColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("Colorform is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([root, a_actor, a_color]() {
			root->UpdateHairColor(a_color->color);

			if (const auto& biped = a_actor->GetCurrentBiped(); biped) {
				for (auto& slot : ACTOR::headSlots) {
					const auto node = biped->objects[slot].partClone;
					if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
						node->UpdateHairColor(a_color->color);
					}
				}
			}

			SET::update_color_data(root, EXTRA::HAIR_TINT, a_color->color);
		});
	}

	inline void SetHeadPartAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		std::int32_t a_type,
		float a_alpha)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (a_type < 0 || a_type > 6) {
			a_vm->TraceStack("Invalid headpart type", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([root, a_actor, a_alpha, a_type]() {
			if (const auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(a_type)); object) {
				object->UpdateMaterialAlpha(a_alpha, false);

				const auto name{ "PO3_HEADPART - " + std::to_string(a_type) };
				if (a_alpha == 1.0f) {
					root->RemoveExtraData(name);
				} else {
					SET::add_data_if_none<RE::NiIntegerExtraData>(root, name, a_type);
				}
			}
		});
	}

	inline void SetHeadPartTextureSet(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSTextureSet* a_txst,
		std::int32_t a_type)
	{
		using HeadPartType = RE::BGSHeadPart::HeadPartType;

		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_txst) {
			a_vm->TraceStack("Textureset is None", a_stackID);
			return;
		}
		if (a_type < 0 || a_type > 6) {
			a_vm->TraceStack("Invalid headpart type", a_stackID);
			return;
		}

		const auto actorBase = a_actor->GetActorBase();
		const auto headpart = actorBase ? actorBase->GetCurrentHeadPartByType(static_cast<HeadPartType>(a_type)) : nullptr;

		if (headpart) {
			headpart->textureSet = a_txst;
		} else {
			a_vm->TraceStack("Could not find matching headpart", a_stackID);
		}
	}

	inline void SetShaderType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		RE::TESObjectREFR* a_template,
		RE::BSFixedString a_filter,
		std::uint32_t a_shaderType,
		std::int32_t a_textureType,
		bool a_noWeapons,
		bool a_noAlpha)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_template) {
			a_vm->TraceStack("Template is None", a_stackID);
			return;
		}
		if (!a_ref->Is3DLoaded()) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}
		if (!a_template->Is3DLoaded()) {
			a_vm->TraceForm(a_template, "template object has no 3D", a_stackID);
			return;
		}

		using Texture = RE::BSTextureSet::Texture;
		using Feature = RE::BSShaderMaterial::Feature;

		auto sourcePath{ std::string() };
		if (!a_filter.empty()) {
			sourcePath = a_filter.c_str();
			TEXTURE::sanitize_path(sourcePath);
		}

		const std::vector<bool> params{ a_noWeapons, a_noAlpha, a_ref->Is(RE::FormType::ActorCharacter) };

		const auto root = a_ref->Get3D()->AsNode();
		const auto template_root = a_template->Get3D()->AsNode();
		const auto feature = static_cast<Feature>(a_shaderType);

		if (root && template_root) {
			SKSE::GetTaskInterface()->AddTask([root, template_root, feature, sourcePath, a_textureType, params]() {
				if (const auto template_geo = template_root->GetFirstGeometryOfShaderType(feature); template_geo) {
					std::vector<RE::BSFixedString> result;
					SET::ShaderType(root, template_geo, sourcePath, a_textureType, result, params);

					if (!result.empty()) {
						std::string name{ "PO3_SHADER | "sv };
						name.append(std::to_string(stl::to_underlying(feature)));

						SET::add_data_if_none<RE::NiStringsExtraData>(root, name, result);
					}
				}
			});
		}
	}

	inline void SetupBodyPartGeometry(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_bodyparts,
		RE::Actor* a_actor)
	{
		if (!a_bodyparts) {
			a_vm->TraceStack("BodyParts is None", a_stackID);
			return;
		}
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_bodyparts->Get3D();
		if (!root) {
			a_vm->TraceForm(a_bodyparts, "has no 3D", a_stackID);
			return;
		}

		const auto actorbase = a_actor->GetActorBase();
		const auto rootNode = root->AsFadeNode();

		if (actorbase && rootNode) {
			const auto actorRoot = a_actor->Get3D(false);
			const auto data = actorRoot ? actorRoot->GetExtraData<RE::NiIntegerExtraData>(EXTRA::SKIN_TINT) : nullptr;

			RE::NiColor color{ data ? data->value : actorbase->bodyTintColor };

			SKSE::GetTaskInterface()->AddTask([root, color]() {
				root->UpdateBodyTint(color);
			});
		}
	}

	inline void SetSkinAlpha(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		float a_alpha)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([root, a_alpha]() {
			root->UpdateMaterialAlpha(a_alpha, true);

			if (a_alpha == 1.0f) {
				root->RemoveExtraData(EXTRA::SKIN_ALPHA);
			} else {
				SET::add_data_if_none<RE::NiBooleanExtraData>(root, EXTRA::SKIN_ALPHA, true);
			}
		});
	}

	inline void SetSkinColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		RE::BGSColorForm* a_color)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("Colorform is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, a_color, root]() {
			SET::tint_face(a_actor, a_color->color);

			root->UpdateBodyTint(a_color->color);

			SET::update_color_data(root, EXTRA::SKIN_TINT, a_color->color);
		});
	}

	inline void ToggleChildNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		std::string a_nodeName,
		bool a_disable)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([root, a_nodeName, a_disable]() {
			if (const auto object = root->GetObjectByName(a_nodeName); object) {
				SET::Toggle(root, object, a_disable);
			}
		});
	}

	inline void ToggleHairWigs(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::Actor* a_actor,
		bool a_disable)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, root, a_disable]() {
			if (const auto& biped = a_actor->GetCurrentBiped(); biped) {
				for (auto& slot : ACTOR::headSlots) {
					const auto node = biped->objects[slot].partClone;
					if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
						SET::Toggle(root, node.get(), a_disable);
					}
				}
			}
		});
	}

	inline void UpdateHitEffectArtNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*,
		RE::TESObjectREFR* a_ref,
		const RE::BGSArtObject* a_art,
		RE::BSFixedString a_toNode,
		std::vector<float> a_translate,
		std::vector<float> a_rotate,
		float a_scale)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_art) {
			a_vm->TraceStack("Art is None", a_stackID);
			return;
		}
		if (a_translate.size() != 3 || a_rotate.size() != 3) {
			a_vm->TraceForm(a_ref, "invalid size", a_stackID);
			return;
		}

		RE::ModelReferenceEffect* hitEffect = nullptr;

		if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
			const auto handle = a_ref->CreateRefHandle();
			processLists->ForEachModelEffect([&](RE::ModelReferenceEffect& a_modelEffect) {
				if (a_modelEffect.target == handle && a_modelEffect.artObject == a_art) {
					hitEffect = &a_modelEffect;
					return RE::BSContainer::ForEachResult::kStop;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		const auto art = hitEffect ? hitEffect->hitEffectArtData.attachedArt : nullptr;

		if (art) {
			RE::NiMatrix3 rotate{};
			const RE::NiPoint3 rot{ RE::deg_to_rad(a_rotate[0]), RE::deg_to_rad(a_rotate[1]), RE::deg_to_rad(a_rotate[2]) };
			rotate.SetEulerAnglesXYZ(rot);

			RE::NiTransform transform;
			transform.translate = { a_translate[0], a_translate[1], a_translate[2] };
			transform.rotate = rotate;
			transform.scale = a_scale;

			SKSE::GetTaskInterface()->AddTask([a_toNode, art, hitEffect, transform]() {
				if (!a_toNode.empty() && hitEffect->hitEffectArtData.nodeName != a_toNode) {
					const auto root = hitEffect->hitEffectArtData.current3DRoot;
					const auto newObj = root ? root->GetObjectByName(a_toNode) : nullptr;
					const auto newNode = newObj ? newObj->AsNode() : nullptr;

					if (newNode) {
						if (const auto attachTData = art->GetExtraData<RE::NiStringsExtraData>("AttachT"sv); attachTData && attachTData->value[0]) {
							std::string newNodeStr{ "NamedNode&" };
							newNodeStr += a_toNode;
							attachTData->Replace(attachTData->value[0], newNodeStr);
						}
						newNode->AttachChild(art.get(), true);
						hitEffect->hitEffectArtData.nodeName = a_toNode;
					}
				}

				for (const auto& nodes : art->children) {
					if (nodes) {
						nodes->local = transform;
					}
				}
			});
		}
	}

	inline void Bind(VM& a_vm)
	{
		BIND(ApplyMaterialShader);
		BIND(BlendColorWithSkinTone);
		BIND(HasSkin);
		BIND(GetHairColor);
		BIND(GetHeadPartTextureSet);
		BIND(GetSkinColor);
		BIND(MixColorWithSkinTone);
		BIND(PlayDebugShader);
		BIND(ReplaceArmorTextureSet);
		BIND(ReplaceFaceTextureSet);
		BIND(ReplaceSkinTextureSet);
		BIND(ResetActor3D);
		BIND(ScaleObject3D);
		BIND(SetHairColor);
		BIND(SetHeadPartAlpha);
		BIND(SetHeadPartTextureSet);
		BIND(SetShaderType);
		BIND(SetupBodyPartGeometry);
		BIND(SetSkinAlpha);
		BIND(SetSkinColor);
		BIND(ToggleChildNode);
		BIND(ToggleHairWigs);
		BIND(UpdateHitEffectArtNode);

		logger::info("Registered graphic functions"sv);
	}
}
