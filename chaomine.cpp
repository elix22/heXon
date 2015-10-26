#include "chaomine.h"
#include "player.h"

ChaoMine::ChaoMine(Context *context, MasterControl *masterControl): Enemy(context, masterControl)
{
    rootNode_->SetName("ChaoMine");
    rigidBody_->SetMass(0.5f);
    meleeDamage_ = 0.1f;
    initialHealth_ = 0.05f;

    innerNode_ = rootNode_->CreateChild();
    innerModel_ = innerNode_->CreateComponent<StaticModel>();
    innerModel_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/MineInner.mdl"));
    innerModel_->SetMaterial(0, masterControl_->resources.materials.shipPrimary);

    outerNode_ = rootNode_->CreateChild();
    outerModel_ = outerNode_->CreateComponent<StaticModel>();
    outerModel_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/MineOuter.mdl"));
    outerModel_->SetMaterial(0, masterControl_->resources.materials.shipSecondary);
    outerModel_->SetMaterial(1, masterControl_->resources.materials.shipPrimary);

}

void ChaoMine::Set(Vector3 position)
{
    Enemy::Set(position);
    SubscribeToEvent(E_SCENEPOSTUPDATE, URHO3D_HANDLER(ChaoMine, HandleMineUpdate));
}

void ChaoMine::HandleMineUpdate(StringHash eventType, VariantMap &eventData)
{
    using namespace ScenePostUpdate;

    float timeStep = eventData[P_TIMESTEP].GetFloat();

    //Spin
    innerNode_->Rotate(Quaternion(50.0f*timeStep, 80.0f*timeStep, 92.0f*timeStep));
    outerNode_->Rotate(Quaternion(-60.0f*timeStep,-101.0f*timeStep, -95.0f*timeStep));
}

void ChaoMine::CheckHealth()
{
    if (rootNode_->IsEnabled() && health_ <= 0 || panicTime_ > 23.0f){
        masterControl_->spawnMaster_->SpawnChaoZap(GetPosition());
        Disable();
    }
}
