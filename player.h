/* heXon
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <Urho3D/Urho3D.h>

#include "sceneobject.h"

#define JOY INPUT->GetJoystickByIndex(playerID_ - 1)

class Bullet;
class Muzzle;
class ChaoFlash;
class TailGenerator;
class Pilot;

class Player : public SceneObject
{
    friend class ChaoMine;
    URHO3D_OBJECT(Player, SceneObject);
public:
    Player(Context* context);
    static void RegisterObject(Context* context);
    virtual void OnNodeSet(Node* node);

    unsigned GetRootNodeID() const { return node_->GetID(); }
    int GetPlayerID() const { return playerID_; }
    Vector3 GetWorldPosition() const { return node_->GetWorldPosition(); }
    void SetPosition(Vector3 pos);
    double GetHealth() const noexcept { return health_; }
    bool IsAlive() const noexcept { return alive_; }
    bool IsActive() const noexcept { return alive_ && IsEnabled(); }
    bool IsHuman() const noexcept { return !autoPilot_; }
    bool IsMoving() const { return rigidBody_->GetLinearVelocity().Length() > 0.01f; }
    void Hit(float damage, bool melee = true);

    void AddScore(int points);
    void Eject();
    void Die();
    void ResetScore();
    unsigned GetScore() const { return score_; }
    unsigned GetFlightScore() const { return flightScore_; }
    void Pickup(PickupType pickup);
    void UpgradeWeapons();

    void EnterLobby();
    void EnterPlay();
    void CreateNewPilot();
    void UpdateGUI(float timeStep);
    void PickupChaoBall();
    void UpdatePilot();
    void KillPilot();
    void SavePilot();
    void ChargeShield();

private:
    int playerID_;
    bool pilotMode_;
    bool autoPilot_;
    Vector3 autoMove_;
    Vector3 autoFire_;
    bool alive_;
    int appleCount_;
    int heartCount_;
    const float initialHealth_;
    float health_;
    unsigned score_;
    unsigned flightScore_;
    unsigned toCount_;
    int multiplier_;
    int weaponLevel_;
    int bulletAmount_;
    float bulletDamage_;

    const float initialShotInterval_;
    float shotInterval_;
    float sinceLastShot_;

    SharedPtr<Pilot> pilot_;
    Node* shieldNode_;
    StaticModel* shieldModel_;
    SharedPtr<Material> shieldMaterial_;
    ChaoFlash* chaoFlash_;
    Vector3 lastHitDirection_;
    RigidBody* rigidBody_;
    CollisionShape* collisionShape_;
    AnimationController* animCtrl_;

    Node* guiNode_;
    Node* scoreNode_;
    Node* healthBarNode_;
    StaticModel* healthBarModel_;
    Node* shieldBarNode_;
    StaticModel* shieldBarModel_;

    Node* appleCounterRoot_;
    Node* appleCounter_[4];
    Node* heartCounterRoot_;
    Node* heartCounter_[4];
    Node* scoreDigits_[10];

    Vector<SharedPtr<TailGenerator> > tailGens_;
    Vector<SharedPtr<Bullet> > bullets_;
    SharedPtr<Muzzle> muzzle_;

    SharedPtr<SoundSource> deathSource_;
    SharedPtr<Sound> shot_s;
    SharedPtr<Sound> shieldHit_s;
    SharedPtr<Sound> death_s;
    Vector<SharedPtr<Sound>> pickup_s;
    SharedPtr<Sound> powerup_s;
    SharedPtr<Sound> multix_s;
    SharedPtr<Sound> chaoball_s;
    Vector<SharedPtr<Sound> > seekerHits_s;

    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
    void Shoot(Vector3 fire);
    void FireBullet(Vector3 direction);
    void SetHealth(float health);
    Color HealthToColor(float health);
    void SetScore(int points);
    void CreateGUI();
    void SetPilotMode(bool pilotMode);
    void MoveMuzzle();
    void LoadPilot();
    void Think();
    Vector3 Sniff(float playerFactor, bool taste = false);
    void CountScore();
};

#endif // PLAYER_H
