#include "Papyrus/Functions/Graphics.h"

#include "Papyrus/Util/ActorGraphics.h"
#include "Papyrus/Util/Graphics.h"

namespace Papyrus::Graphics
{
	using namespace GRAPHICS;

	void ApplyMaterialShader(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::BGSMaterialObject* a_shader, float a_materialThresholdAngle)
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

		SKSE::GetTaskInterface()->AddTask([a_shader, a_materialThresholdAngle, root]() {
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
		});
	}

	void BlendColorWithSkinTone(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color, std::uint32_t a_blendMode, bool a_autoCalc, float a_opacity)
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
			auto        newColor = RE::ColorUtil::Blend(actorbase->bodyTintColor, a_color->color, static_cast<BLEND_MODE>(a_blendMode), opacity);

			SKSE::GetTaskInterface()->AddTask([a_actor, root, newColor]() {
				ActorApplier::BodyTint(a_actor, root, newColor);
			});
		}
	}

	bool HasSkin(STATIC_ARGS, RE::Actor* a_actor, RE::TESObjectARMO* a_check)
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

	RE::BGSColorForm* GetHairColor(STATIC_ARGS, [[maybe_unused]] const RE::Actor* a_actor)
	{
		a_vm->TraceStack("Function is deprecated. Use GetHairRGB instead", a_stackID);
		return nullptr;
	}

	std::vector<std::uint32_t> GetHairRGB(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return {};
		}

		RE::Color color;

		const auto root = a_actor->Get3D(false);
		const auto data = root ? root->GetExtraData<RE::NiIntegerExtraData>(EXTRA::HAIR_TINT) : nullptr;

		if (data) {
			color = RE::Color(data->value);
		} else {
			const auto actorbase = a_actor->GetActorBase();
			if (const auto headData = actorbase ? actorbase->headRelatedData : nullptr; headData && headData->hairColor) {
				color = headData->hairColor->color;
			}
		}

		return { static_cast<std::uint32_t>(color.red), static_cast<std::uint32_t>(color.green), static_cast<std::uint32_t>(color.blue) };
	}

	RE::BGSTextureSet* GetHeadPartTextureSet(STATIC_ARGS, RE::Actor* a_actor, std::uint32_t a_type)
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

	RE::BGSColorForm* GetSkinColor(STATIC_ARGS, [[maybe_unused]] const RE::Actor* a_actor)
	{
		a_vm->TraceStack("Function is deprecated. Use GetSkinRGB instead", a_stackID);
		return nullptr;
	}

	std::vector<std::uint32_t> GetSkinRGB(STATIC_ARGS, const RE::Actor* a_actor)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return {};
		}

		RE::Color color;

		const auto root = a_actor->Get3D(false);
		const auto data = root ? root->GetExtraData<RE::NiIntegerExtraData>(EXTRA::SKIN_TINT) : nullptr;
		if (data) {
			color = RE::Color(data->value);
		} else {
			if (auto actorBase = a_actor->GetActorBase()) {
				color = actorBase->bodyTintColor;
			}
		}

		return { static_cast<std::uint32_t>(color.red), static_cast<std::uint32_t>(color.green), static_cast<std::uint32_t>(color.blue) };
	}

	void MixColorWithSkinTone(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color, bool a_manual, float a_percent)
	{
		a_vm->TraceStack("Function is deprecated, use BlendColorWithSkinTone instead", a_stackID);

		if (a_actor && a_color) {
			const auto actorbase = a_actor->GetActorBase();
			const auto root = a_actor->Get3D(false);
			if (actorbase && root) {
				const float skinLuminance = a_manual ? a_percent : RE::ColorUtil::CalcLuminance(actorbase->bodyTintColor);
				auto        newColor = RE::ColorUtil::Mix(actorbase->bodyTintColor, a_color->color, skinLuminance);

				SKSE::GetTaskInterface()->AddTask([a_actor, newColor, root]() {
					ActorApplier::BodyTint(a_actor, root, newColor);
				});
			}
		}
	}

	void PlayDebugShader(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::vector<float> a_rgba)
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

	void ReplaceArmorTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::TESObjectARMO* a_armor, RE::BGSTextureSet* a_srcTXST, RE::BGSTextureSet* a_tgtTXST, std::int32_t a_type)
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
					ActorApplier::ArmorTXST(&a_obj, a_tgtTXST, a_type, targetPath, replaced);
					return true;
				});
			}

			const auto root = a_actor->Get3D(false);
			if (replaced && root) {
				auto       armorID = std::to_string(a_armor->formID);
				const auto name = "PO3_TXST - " + armorID;

				const auto data = root->GetExtraData<RE::NiStringsExtraData>(name);
				if (!data) {
					std::vector<RE::BSFixedString> results;
					results.resize(Texture::kTotal + 1);
					for (const auto type : stl::enum_range(Texture::kDiffuse, Texture::kTotal)) {
						results[type] = (a_srcTXST->GetTexturePath(type));
					}
					results[Texture::kTotal] = armorID;
					if (const auto newData = RE::NiStringsExtraData::Create(name, results); newData) {
						root->AddExtraData(newData);
					}
				}
			}
		});
	}

	void ReplaceFaceTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::int32_t a_type)
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
					std::vector<RE::BSFixedString> results;
					ActorApplier::SkinTXST(faceObject, txst, results, a_type);
					const auto root = a_actor->Get3D(false);
					if (!results.empty() && root) {
						EXTRA::Add<RE::NiStringsExtraData>(root, EXTRA::FACE_TXST, results);
					}
				}
			});
		}
	}

	void ReplaceSkinTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::BGSTextureSet* a_maleTXST, RE::BGSTextureSet* a_femaleTXST, std::uint32_t a_slot, std::int32_t a_type)
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

		ActorApplier::ArmorSkinTXST(a_actor,
			isFemale ? a_femaleTXST : a_maleTXST,
			static_cast<BipedSlot>(a_slot), a_type);
	}

	bool ResetActor3D(STATIC_ARGS, RE::Actor* a_actor, RE::BSFixedString a_folderName)
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

		SKSE::GetTaskInterface()->AddTask([a_actor, root, a_folderName]() {
			ActorResetter resetter(a_actor, root, a_folderName);
			resetter.ResetActor3D();
		});

		return true;
	}

	void ScaleObject3D(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName, float a_scale)
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

	void SetHairColor(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}
		if (!a_color) {
			a_vm->TraceStack("ColorForm is None", a_stackID);
			return;
		}

		const auto root = a_actor->Get3D(false);
		if (!root) {
			a_vm->TraceForm(a_actor, "has no 3D", a_stackID);
			return;
		}

		SKSE::GetTaskInterface()->AddTask([a_actor, a_color, root]() {
			ActorApplier::HairTint(a_actor, root, a_color->color);
		});
	}

	void SetHeadPartAlpha(STATIC_ARGS, RE::Actor* a_actor, std::int32_t a_type, float a_alpha)
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
					EXTRA::Add<RE::NiIntegerExtraData>(root, name, a_type);
				}
			}
		});
	}

	void SetHeadPartTextureSet(STATIC_ARGS, RE::Actor* a_actor, RE::BGSTextureSet* a_txst, std::int32_t a_type)
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

	void SetShaderType(STATIC_ARGS, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_template, RE::BSFixedString a_filter, std::uint32_t a_shaderType, std::int32_t a_textureType, bool a_noWeapons, bool a_noAlpha)
	{
		if (!a_ref) {
			a_vm->TraceStack("Object reference is None", a_stackID);
			return;
		}
		if (!a_template) {
			a_vm->TraceStack("Template is None", a_stackID);
			return;
		}

		const auto root = a_ref->Get3D();
		if (!root) {
			a_vm->TraceForm(a_ref, "has no 3D", a_stackID);
			return;
		}

		const auto templateRoot = a_template->Get3D();
		if (!templateRoot) {
			a_vm->TraceForm(a_template, "has no 3D", a_stackID);
			return;
		}

		ShaderData::Input inputData(a_ref, a_filter, a_shaderType, a_textureType, a_noWeapons, a_noAlpha);

		SKSE::GetTaskInterface()->AddTask([root, templateRoot, inputData]() {
			inputData.SetShaderType(root, templateRoot);
		});
	}

	void SetupBodyPartGeometry(STATIC_ARGS, RE::TESObjectREFR* a_bodyparts, RE::Actor* a_actor)
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

	void SetSkinAlpha(STATIC_ARGS, RE::Actor* a_actor, float a_alpha)
	{
		if (!a_actor) {
			a_vm->TraceStack("Actor is None", a_stackID);
			return;
		}

		for (std::uint32_t i = 0; i < 2; i++) {
			if (const auto root = a_actor->Get3D(static_cast<bool>(i))) {
				SKSE::GetTaskInterface()->AddTask([root, a_alpha]() {
					root->UpdateMaterialAlpha(a_alpha, true);
					if (a_alpha == 1.0f) {
						root->RemoveExtraData(EXTRA::SKIN_ALPHA);
					} else {
						EXTRA::Add<RE::NiBooleanExtraData>(root, EXTRA::SKIN_ALPHA, true);
					}
				});
			}
		}
	}

	void SetSkinColor(STATIC_ARGS, RE::Actor* a_actor, RE::BGSColorForm* a_color)
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
			ActorApplier::BodyTint(a_actor, root, a_color->color);
		});
	}

	void ToggleChildNode(STATIC_ARGS, RE::TESObjectREFR* a_ref, std::string a_nodeName, bool a_disable)
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
				ActorApplier::ToggleNode(root, object, a_disable);
			}
		});
	}

	void ToggleHairWigs(STATIC_ARGS, RE::Actor* a_actor, bool a_disable)
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
				for (auto& slot : headSlots) {
					const auto& node = biped->objects[slot].partClone;
					if (node && node->HasShaderType(RE::BSShaderMaterial::Feature::kHairTint)) {
						ActorApplier::ToggleNode(root, node.get(), a_disable);
					}
				}
			}
		});
	}

	void UpdateHitEffectArtNode(STATIC_ARGS, RE::TESObjectREFR* a_ref, const RE::BGSArtObject* a_art, RE::BSFixedString a_toNode, std::vector<float> a_translate, std::vector<float> a_rotate, float a_scale)
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
			processLists->ForEachModelEffect([&](RE::ModelReferenceEffect* a_modelEffect) {
				if (a_modelEffect->target == handle && a_modelEffect->artObject == a_art) {
					hitEffect = a_modelEffect;
					return RE::BSContainer::ForEachResult::kStop;
				}
				return RE::BSContainer::ForEachResult::kContinue;
			});
		}

		const auto art = hitEffect ? hitEffect->hitEffectArtData.attachedArt : nullptr;

		if (art) {
			RE::NiMatrix3      rotate{};
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

	void Bind(VM& a_vm)
	{
		BIND(ApplyMaterialShader);
		BIND(BlendColorWithSkinTone);
		BIND(HasSkin);
		BIND(GetHairColor);
		BIND(GetHairRGB);
		BIND(GetHeadPartTextureSet);
		BIND(GetSkinColor);
		BIND(GetSkinRGB);
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
