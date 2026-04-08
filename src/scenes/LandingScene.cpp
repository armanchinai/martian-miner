//
// Created by qthompson on 2026-03-24.
//

#include "LandingScene.h"

#include <sstream>

#include "managers/AssetManager.h"
#include "utils/Collision.h"

LandingScene::LandingScene(const char* name, const int windowWidth, const int windowHeight) :
Scene(name, windowWidth, windowHeight, "../assets/martianValleys2.tmx", "../assets/mars_tileset_w_background.png",
    {
        {8, 8, 16, 16},
        {8 + 32, 8, 16, 16},
        {8 + 32 * 2, 8, 16, 16},
        {8 + 32 * 3, 8, 16, 16},
        {8 + 32 * 4, 8, 16, 16},
        {8, 8 + 32, 16, 16},
        {8 + 32, 8 + 32, 16, 16},
        {8 + 32 * 2, 8 + 32, 16, 16},
        {8 + 32 * 3, 8 + 32, 16, 16},
        {8 + 32 * 4, 8 + 32, 16, 16},
        {8, 8 + 32 * 2, 16, 16},
        {8 + 32, 8 + 32 * 2, 16, 16},
        {8 + 32 * 2, 8 + 32 * 2, 16, 16},
        {8 + 32 * 3, 8 + 32 * 2, 16, 16},
        {8 + 32 * 4, 8 + 32 * 2, 16, 16},
        {8, 8 + 32 * 3, 16, 16},
        {8 + 32, 8 + 32 * 3, 16, 16},
        {8 + 32 * 2, 8 + 32 * 3, 16, 16},
        {8 + 32 * 3, 8 + 32 * 3, 16, 16},
        {8 + 32 * 4, 8 + 32 * 3, 16, 16},
        {8, 8 + 32 * 4, 16, 16},
        {8 + 32, 8 + 32 * 4, 16, 16},
        {8 + 32 * 2, 8 + 32 * 4, 16, 16},
        {8 + 32 * 3, 8 + 32 * 4, 16, 16},
        {8 + 32 * 4, 8 + 32 * 4, 16, 16},
        {8, 8 + 32 * 5, 16, 16},
        {8 + 32, 8 + 32 * 5, 16, 16},
        {8 + 32 * 2, 8 + 32 * 5, 16, 16},
        {8 + 32 * 3, 8 + 32 * 5, 16, 16},
        {8 + 32 * 4, 8 + 32 * 5, 16, 16},
    })
{
    // Load Fonts
    AssetManager::loadFont("OCRA", "../assets/fonts/OCRA.ttf", 16.0f);
    AssetManager::loadFont("OCRA-Header", "../assets/fonts/OCRA.ttf", 24.0f);

    for (const SDL_FRect& rect : world.getMap().landingZones)
    {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(rect.x, rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("landingZone");
        c.rect.x = rect.x;
        c.rect.y = rect.y;
        c.rect.w = rect.w;
        c.rect.h = rect.h;

        SDL_Texture* colTex = TextureManager::load("../assets/gizmos.png");
        SDL_FRect colTexSrc = {33, 1, 30, 30};
        SDL_FRect colTexDst = {rect.x, rect.y, rect.w, rect.h};
        e.addComponent<Sprite>(colTex, colTexSrc, colTexDst);
    }

    auto& leftBarrier(world.createEntity());
    leftBarrier.addComponent<Transform>(Vector2D(-20.f, 0), 0.0f, 1.0f);
    auto& lCol = leftBarrier.addComponent<Collider>("barrier");
    lCol.rect.w = 20.0f;
    lCol.rect.h = world.getMap().height * world.getMap().tileHeight;

    auto& rightBarrier(world.createEntity());
    rightBarrier.addComponent<Transform>(Vector2D(world.getMap().width * world.getMap().tileWidth, 0), 0.0f, 1.0f);
    auto& rCol = rightBarrier.addComponent<Collider>("barrier");
    rCol.rect.w = 20.0f;
    rCol.rect.h = world.getMap().height * world.getMap().tileHeight;

    auto& topBarrier(world.createEntity());
    topBarrier.addComponent<Transform>(Vector2D(0, -21), 0.0f, 1.0f);
    auto& tCol = topBarrier.addComponent<Collider>("barrier");
    tCol.rect.h = 20.0f;
    tCol.rect.w = world.getMap().width * world.getMap().tileWidth;

    auto& bottomBarrier(world.createEntity());
    bottomBarrier.addComponent<Transform>(Vector2D(0, world.getMap().height * world.getMap().tileHeight + 1), 0.0f, 1.0f);
    auto& bCol = bottomBarrier.addComponent<Collider>("barrier");
    bCol.rect.h = 20.0f;
    bCol.rect.w = world.getMap().width * world.getMap().tileWidth;

    auto& player(world.createEntity());
    player.addComponent<PlayerTag>();

    const auto& playerT = player.addComponent<Transform>(Vector2D(world.getMap().width * world.getMap().tileWidth / 2,10.0f), 0.0f, 1.0f);

    player.addComponent<Velocity>(Vector2D(0.0f, 1.0f), 200.0f);
    player.addComponent<Acceleration>(Vector2D(0.0f, 0.0f));
    auto& physObj = player.addComponent<PhysicsObject>();
    physObj.angle = 0.0f;
    player.addComponent<ForceInput>();

    auto& points = player.addComponent<Points>();
    points.target = world.getMap().landingZones.size();

    // Load Animations
    AssetManager::loadAnimation("player", "../assets/animations/lander_animations.xml");
    AssetManager::loadAnimation("explosion", "../assets/animations/explosion_animations.xml");
    AssetManager::loadAnimation("asteroids", "../assets/animations/asteroid_animations.xml");

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../assets/animations/lander1.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerT.position.x, playerT.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);

    auto& playerCol = player.addComponent<Collider>("player");
    playerCol.rect.w = playerDst.w;
    playerCol.rect.h = playerDst.h;

    // Setup UI

    auto& labelEntity(world.createEntity());
    labelEntity.addComponent<Transform>(Vector2D(10.0f, 10.0f), 0.0f, 1.0f);
    Label scoreLabel = {
        "Landings: 0/" + std::to_string(points.target),
        AssetManager::getFont("OCRA"),
        {255, 255, 255, 255},
        "scoreCounter",
    };
    TextureManager::loadLabel(scoreLabel);
    labelEntity.addComponent<Label>(scoreLabel);

    createGameOverOverlay(windowWidth, windowHeight);
    createOverlayComponents();

    auto& asteroidSpawner(world.createEntity());
    auto t = asteroidSpawner.addComponent<Transform>(Vector2D(3750.0f, 48.0f), 0.0f, 1.0f);
    asteroidSpawner.addComponent<TimedSpawner>(0.5f, [this, t]
    {
        auto& asteroid(world.createDeferredEntity());
        asteroid.addComponent<ProjectileTag>();
        asteroid.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        asteroid.addComponent<Velocity>(Vector2D(-randomFloat(), randomFloat()), 100.0f);

        Animation asteroidAnim = AssetManager::getAnimation("asteroids");
        asteroid.addComponent<Animation>(asteroidAnim);

        SDL_Texture* asteroidTex = TextureManager::load("../assets/animations/asteroid.png");
        SDL_FRect src {0, 0, 32, 32};
        SDL_FRect dst {t.position.x, t.position.y, 32, 32};
        asteroid.addComponent<Sprite>(asteroidTex, src, dst);

        Collider c = asteroid.addComponent<Collider>("asteroid", dst);
    });

    // Mouse Interaction Listener
    world.getEventManager().subscribe([&](const BaseEvent& e) { // Should be made default behaviour in scene
        if (e.type != EventType::MouseInteraction) {
            return;
        }

        const auto mouseInteractionEvent = dynamic_cast<const MouseInteractionEvent&>(e);

        if (!mouseInteractionEvent.entity->hasComponent<Clickable>()) {
            return;
        }

        auto& clickable = mouseInteractionEvent.entity->getComponent<Clickable>();
        auto& collider = mouseInteractionEvent.entity->getComponent<Collider>();

        if (!collider.enabled) {
            return;
        }

        switch (mouseInteractionEvent.state) {
            case MouseInteractionState::Pressed:
                clickable.onPressed();
                break;
            case MouseInteractionState::Released:
                clickable.onReleased();
                break;
            case MouseInteractionState::Cancelled:
                clickable.onCancelled();
        }
    });

    // Landing Zone Listener
    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::Collision) {
            return;
        }

        const auto collisionEvent = dynamic_cast<const CollisionEvent &>(e);

        Entity* entity = nullptr;
        Entity* other = nullptr;

        if (!Collision::getCollisionParticipants(collisionEvent, "player", "landingZone", entity, other))
        {
            return;
        }

        if (collisionEvent.state == CollisionState::Enter)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                auto& playerTag = entity->getComponent<PlayerTag>();
                playerTag.withinLandingZone = true;
            }
        }
        else if (collisionEvent.state == CollisionState::Exit)
        {
            if (entity->hasComponent<PlayerTag>())
            {
                auto& playerTag = entity->getComponent<PlayerTag>();
                playerTag.withinLandingZone = false;
                other->destroy();
                world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Interact});
            }
        }
    });

    // Barrier/Wall Listener
    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::Collision) {
            return;
        }

        const auto collisionEvent = dynamic_cast<const CollisionEvent &>(e);

        Entity* entity = nullptr;
        Entity* other = nullptr;

        bool isWall = false;
        bool isAsteroid = false;
        if (Entity* wall = nullptr; Collision::getCollisionParticipants(collisionEvent, "player", "wall", entity, wall))
        {
            other = wall;
            isWall = true;
        }
        else if (Entity* barrier = nullptr; Collision::getCollisionParticipants(collisionEvent, "player", "barrier", entity, barrier))
        {
            other = barrier;
        }
        else if (Entity* asteroid = nullptr; Collision::getCollisionParticipants(collisionEvent, "player", "asteroid", entity, asteroid)) {
            other = asteroid;
            isAsteroid = true;
        } else if (Collision::getCollisionParticipants(collisionEvent, "wall", "asteroid", entity, asteroid)) {
            isAsteroid = true;
            isWall = true;
            asteroid->destroy();
            return;
        } else {
            return;
        }

        auto& t = entity->getComponent<Transform>();
        auto& v = entity->getComponent<Velocity>();
        auto& playerRect = entity->getComponent<Collider>().rect;
        auto& wallRect   = other->getComponent<Collider>().rect;

        const float playerCenterX = playerRect.x + playerRect.w  * 0.5f;
        const float playerCenterY = playerRect.y + playerRect.h * 0.5f;

        const float wallCenterX = wallRect.x + wallRect.w  * 0.5f;
        const float wallCenterY = wallRect.y + wallRect.h * 0.5f;

        const float playerHalfW = playerRect.w  * 0.5f;
        const float playerHalfH = playerRect.h * 0.5f;

        const float wallHalfW = wallRect.w  * 0.5f;
        const float wallHalfH = wallRect.h * 0.5f;

        const float dx = playerCenterX - wallCenterX;
        const float dy = playerCenterY - wallCenterY;

        const float overlapX = playerHalfW + wallHalfW - std::abs(dx);

        if (const float overlapY = playerHalfH + wallHalfH - std::abs(dy); overlapX > 0.0f && overlapY > 0.0f)
        {
            if (overlapX < overlapY)
            {
                if (dx > 0) {
                    t.position.x += overlapX;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.x)*(v.magnitude*v.direction.x));
                    v.direction.x = 0;
                    if (v.direction.y < 0) {
                        v.direction.y = -1;
                    } else {
                        v.direction.y = 1;
                    }
                }
                else {
                    t.position.x -= overlapX;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.x)*(v.magnitude*v.direction.x));
                    v.direction.x = 0;
                    if (v.direction.y < 0) {
                        v.direction.y = -1;
                    } else {
                        v.direction.y = 1;
                    }
                }
            }
            else
            {
                if (dy > 0) {
                    t.position.y += overlapY;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.y)*(v.magnitude*v.direction.y));
                    v.direction.y = 0;
                    if (v.direction.x < 0) {
                        v.direction.x = -1;
                    } else {
                        v.direction.x = 1;
                    }
                }
                else {
                    t.position.y -= overlapY;
                    v.magnitude = std::sqrt((v.magnitude*v.magnitude) - (v.magnitude*v.direction.y)*(v.magnitude*v.direction.y));
                    v.direction.y = 0;
                    v.direction.x = 0;
                    v.magnitude = 0;
                }
            }

            playerRect.x = t.position.x;
            playerRect.y = t.position.y;
        }

        if (isWall)
        {
            auto& tag = entity->getComponent<PlayerTag>();
            if (!tag.withinLandingZone) {
                world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
            } else if (int angle = (static_cast<int>(t.rotation) % 360 + 360) % 360; (angle > 15 && angle < 345) || v.magnitude > 100.0f) {
                world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
            }
        }
        if (isAsteroid) {
            world.getEventManager().emit(PlayerActionEvent{entity, PlayerAction::Death});
        }
    });

    // PlayerAction
    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::PlayerAction)
        {
            return;
        }

        const auto& actionEvent = static_cast<const PlayerActionEvent&>(e);

        if (actionEvent.action == PlayerAction::Interact)
        {
            auto& p = player.getComponent<Points>();
            p.current += 1;

            std::stringstream ss;
            ss << "Landings: " << p.current << "/" << p.target;
            auto& label = labelEntity.getComponent<Label>();
            label.text = ss.str();
            label.dirty = true;


        } else if (actionEvent.action == PlayerAction::Death) {
            if (gameOver == true)
            {
                return;
            }
            auto &playerExplosion (world.createDeferredEntity());
            Animation explosionAnim = AssetManager::getAnimation("explosion");
            auto explosionT = playerExplosion.addComponent<Transform>(Vector2D(playerT.position.x, playerT.position.y), 0.0f, 1.0f);
            SDL_Texture* explosionTex = TextureManager::load("../assets/animations/explosion_anim.png");
            SDL_FRect explosionSrc = explosionAnim.clips[explosionAnim.currentClip].frameIndices[0];
            SDL_FRect explosionDst {explosionT.position.x, explosionT.position.y, 64, 64};
            playerExplosion.addComponent<Sprite>(explosionTex, explosionSrc, explosionDst);
            auto& animComponent = playerExplosion.addComponent<Animation>(explosionAnim);
            animComponent.looping = false;
            world.getEventManager().emit(GameStateEvent{GameState::Lose});
        }
    });

    world.getEventManager().subscribe([&](const BaseEvent& e) {
        if (e.type != EventType::GameState) {
            return;
        }

        if (gameOver == true)
        {
            return;
        }

        const auto& gameStateEvent = static_cast<const GameStateEvent&>(e);

        if (gameStateEvent.gameState == GameState::Win) {
            updateOverlayComponents(true);
            toggleOverlayVisibility();
        } else if (gameStateEvent.gameState == GameState::Lose) {
            updateOverlayComponents(false);
            toggleOverlayVisibility();
        }
        gameOver = true;
    });

    // Player Keyboard Input
    world.getEventManager().subscribe([&](const BaseEvent& e)
    {
        if (e.type != EventType::KeyboardInteraction)
        {
            return;
        }

        if (gameOver)
        {
            return;
        }

        const auto& keyboardEvent = static_cast<const KeyboardInteractionEvent&>(e);

        auto& fi = player.getComponent<ForceInput>();
        if (keyboardEvent.state == KeyboardInteractionState::Pressed)
        {
            switch (keyboardEvent.key)
            {
            case SDLK_W:
                fi.inputPositional.y = -100;
                break;
            case SDLK_A:
                fi.inputPositional.x = -100;
                break;
            case SDLK_D:
                fi.inputPositional.x = 100;
                break;
            default:
                break;
            }
        }
        else if (keyboardEvent.state == KeyboardInteractionState::Released)
        {
            switch (keyboardEvent.key)
            {
            case SDLK_W:
                fi.inputPositional.y = 0;
                break;
            case SDLK_A:
                fi.inputPositional.x = 0;
                fi.inputAngular = 0;
                break;
            case SDLK_D:
                fi.inputPositional.x = 0;
                fi.inputAngular = 0;
                break;
            default:
                break;
            }
        }
    });

    // GameOver player remover
    world.getEventManager().subscribe([&](const BaseEvent& e)
    {
        if (gameOver)
        {
            player.deactivateComponent<ForceInput>();
            player.deactivateComponent<PhysicsObject>();
            player.deactivateComponent<Velocity>();
            player.deactivateComponent<Acceleration>();
            player.deactivateComponent<Sprite>();
            player.deactivateComponent<Animation>();
            player.deactivateComponent<Collider>();
        }
    });
}

SDL_FColor lerp(const SDL_FColor& a, const SDL_FColor& b, float t)
{
    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        1.0f
    };
}

float LandingScene::randomFloat()
{
    return static_cast<float>(rand()) / static_cast<float>((RAND_MAX));
}

SDL_FColor LandingScene::getBackgroundColour()
{
    for (auto& entity : world.getEntities())
    {
        if (playerEntity && cameraEntity)
        {
            break;
        }

        if (entity->hasComponent<Camera>())
        {
            cameraEntity = entity.get();
        }
        else if (entity->hasComponent<PlayerTag>())
        {
            playerEntity = entity.get();
        }
    }

    if (playerEntity && cameraEntity)
    {
        constexpr SDL_FColor skyColour = {0.1f, 0.0f, 0.2f, 1.0f};
        constexpr SDL_FColor groundColour = {0.95f, 0.80f, 0.55f, 1.0f};
        constexpr SDL_FColor underGroundColour = {0.16f, 0.0f, 0.01f, 1.0f};

        const auto& playerTransform = playerEntity->getComponent<Transform>();
        const auto& camera = cameraEntity->getComponent<Camera>();

        if (const float heightPercent = playerTransform.position.y / camera.worldHeight; heightPercent < 0.20f)
        {
            const float blend = heightPercent / 0.20f;
            return lerp(skyColour, groundColour, blend);
        }
        else {
            if (heightPercent < 0.90f)
            {
                const float blend = (heightPercent - 0.20f) / 0.70f;
                return lerp(groundColour, underGroundColour, blend);
            }
            const float blend = (heightPercent - 0.90f) / 0.10f;
            return lerp(underGroundColour, underGroundColour, blend);
        }
    }
    return {0.0f, 0.0f, 0.0f, 1.0f};
}

void LandingScene::createGameOverOverlay(int windowWidth, int windowHeight)
{
    overlayEntity = &world.createEntity();
    SDL_Texture *panelTex = TextureManager::load("../assets/martian_miner_bg.png");
    SDL_FRect panelSrc {0, 0 , 1280.0f, 960.0f};
    auto& overlayT = overlayEntity->addComponent<Transform>(Vector2D((float)windowWidth/2 - panelSrc.w/6, 50.0f), 0.0f, 1.0f);
    SDL_FRect panelDst {overlayT.position.x, overlayT.position.y, panelSrc.w/3, panelSrc.h/3};
    overlayEntity->addComponent<Sprite>(panelTex, panelSrc, panelDst, RenderLayer::UI, false);
}

void LandingScene::createOverlayComponents()
{
    if (!overlayEntity->hasComponent<Children>()) {
        overlayEntity->addComponent<Children>();
    }
    auto& overlayTransform = overlayEntity->getComponent<Transform>();
    auto& overlaySprite = overlayEntity->getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    float baseY = overlayTransform.position.y;

    auto& restartButton = world.createEntity();

    SDL_Texture *symbolsTex = TextureManager::load("../assets/ui_symbols.png");
    SDL_FRect restartButtonSrc = {64, 0, 64, 64};

    auto& restartButtonT = restartButton.addComponent<Transform>(Vector2D(baseX + overlaySprite.dst.w/2 - restartButtonSrc.w/2, baseY + overlaySprite.dst.h - 100), 0.0f, 1.0f);

    SDL_FRect restartButtonDst = {restartButtonT.position.x, restartButtonT.position.y, restartButtonSrc.w, restartButtonSrc.h};
    restartButton.addComponent<Sprite>(symbolsTex, restartButtonSrc, restartButtonDst, RenderLayer::UI, false);
    auto& col = restartButton.addComponent<Collider>("ui", restartButtonDst);
    col.enabled = false;
    auto& clickable = restartButton.addComponent<Clickable>();
    clickable.onPressed = [&restartButtonT] {
        restartButtonT.scale = 0.75f;
    };

    clickable.onReleased = [this, &restartButtonT] {
        restartButtonT.scale = 1.0f;
        world.getEventManager().emit(SceneSwapEvent("game"));
    };

    clickable.onCancelled = [&restartButtonT] {
        restartButtonT.scale = 1.0f;
    };

    restartButton.addComponent<Parent>(overlayEntity);
    auto& parentChildren = overlayEntity->getComponent<Children>();

    titleEntity = &world.createEntity();
    titleEntity->addComponent<Transform>(Vector2D(baseX + 10, baseY + 10), 0.0f, 1.0f);

    scoreEntity = &world.createEntity();
    scoreEntity->addComponent<Transform>(Vector2D(baseX, baseY + 100), 0.0f, 1.0f);

    parentChildren.children.push_back(&restartButton);
    parentChildren.children.push_back(titleEntity);
    parentChildren.children.push_back(scoreEntity);
}

void LandingScene::updateOverlayComponents(bool isWin) {
    auto& overlayTransform = overlayEntity->getComponent<Transform>();
    auto& overlaySprite = overlayEntity->getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    auto& titleT = titleEntity->getComponent<Transform>();
    auto& scoreT = scoreEntity->getComponent<Transform>();

    Label title, score;
    if (!playerEntity)
    {
        for (auto& e : world.getEntities())
        {
            if (e->hasComponent<PlayerTag>())
            {
                playerEntity = e.get();
            }
        }
    }

    auto& [current, target] = playerEntity->getComponent<Points>();

    if (isWin)
    {
        title = {
            "[Mission Successful]",
            AssetManager::getFont("OCRA-Header"),
            {0, 255, 0, 255},
            "winTitle",
        };
    }
    else
    {
        title = {
            "[Mission Failed]",
            AssetManager::getFont("OCRA-Header"),
            {255, 0, 0, 255},
            "loseTitle",
        };
    }
    score = {
        std::to_string(current) + " of " + std::to_string(target) + " sites visited.",
        AssetManager::getFont("OCRA"),
        {255, 255, 255, 255},
        "loseScore"
    };

    TextureManager::loadLabel(title);
    TextureManager::loadLabel(score);
    float w, h;
    SDL_GetTextureSize(title.texture, &w, &h);

    title.visible = false;
    titleEntity->addComponent<Label>(title);
    titleT.position.x = baseX + overlaySprite.dst.w/2 - w/2;

    SDL_GetTextureSize(score.texture, &w, &h);
    score.visible = false;
    scoreEntity->addComponent<Label>(score);
    scoreT.position.x = baseX + overlaySprite.dst.w/2 - w/2;
}

void LandingScene::toggleOverlayVisibility()
{
    auto &sprite = overlayEntity->getComponent<Sprite>();
    bool newVis = !sprite.visible;
    sprite.visible = newVis;

    if (overlayEntity->hasComponent<Children>()) {
        auto&[children] = overlayEntity->getComponent<Children>();
        for (const auto& child : children) {
            if (child && child->hasComponent<Sprite>()) {
                auto& cSprite = child->getComponent<Sprite>();
                cSprite.visible = newVis;
            }
            else if (child && child->hasComponent<Label>())
            {
                auto& cLabel = child->getComponent<Label>();
                cLabel.visible = newVis;
            }

            if (child && child->hasComponent<Collider>()) {
                auto& collider = child->getComponent<Collider>();
                collider.enabled = newVis;
            }
        }
    }
}
