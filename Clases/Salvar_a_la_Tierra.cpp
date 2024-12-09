#include "Salvar_a_la_Tierra.h"
#include "audio/include/AudioEngine.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\GameSettings.h"
#include "C:\Users\Danie\Desktop\AstroPilot\proj.win32\leaderboard.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* Salvar_a_la_Tierra::createScene()
{
    return Salvar_a_la_Tierra::create();
}

// Constructor
Salvar_a_la_Tierra::Salvar_a_la_Tierra() : _nave(nullptr),boss(nullptr),bossVida(0), fondoActual(nullptr) {}

// Destructor
Salvar_a_la_Tierra::~Salvar_a_la_Tierra() {}

// Inicializador
bool Salvar_a_la_Tierra::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    audioFondo = AudioEngine::play2d("Song.mp3", true);
    float currentVolume = GameSettings::getMusicVolume();
    AudioEngine::setVolume(audioFondo, currentVolume); // 50% de volumen

    // Inicializar el puntaje
    score = 0;

    // Etiqueta del puntaje
    scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
    scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 20)); // Centrado arriba
    addChild(scoreLabel, 5);

    // Fondo de la escena
    fondoActual = Sprite::create("FondoSalvar.png");
    fondoActual->setAnchorPoint(Vec2(0, 0));
    fondoActual->setPosition(Vec2(0, 0));
    addChild(fondoActual, 0); // Agregar el fondo inicial

    // Crear la nave
    _nave = Sprite::create("Nave.png");
    _nave->setAnchorPoint(Vec2(0, 0));
    _nave->setPosition(Vec2(visibleSize.width / 2, 50));
    addChild(_nave, 2);
    //Incializar Vidas
    inicializarVidas();
    // Inicializar controles de movimiento
    inicializarMovimiento();

    // Actualizaciones continuas
    scheduleUpdate();
    //schedule(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::crearEnemigos), 5.0f);
    scheduleOnce(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::juego), 3.0f); //Juego Principal
    //scheduleOnce(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::crearBoss), 0.0f);
    //scheduleOnce(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::crearBoss3), 2.0f);

    return true;
}

//Manejo de pauda
void Salvar_a_la_Tierra::Pausa(Ref* pSender) {
    if (Director::getInstance()->isPaused()) {
        this->removeChildByName("pauseMenu");
        this->removeChildByName("volumenLabel");
        Director::getInstance()->resume(); // Reanuda el juego
    }
    else {
        Director::getInstance()->pause(); // Pausa el juego
    }
}
//Para ir al menu
void Salvar_a_la_Tierra::GoToMenu(Ref* pSender) {
    Director::getInstance()->resume();
    AudioEngine::stop(audioFondo);
    auto HelloScene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HelloScene, Color3B(0, 0, 0)));
}

// Metodo para disparar un misil
void Salvar_a_la_Tierra::lanzarMisil()
{
    auto misil = Sprite::create("Misil.png");
    misil->setName("Misil");

    misil->setPosition(_nave->getPosition() + Vec2(_nave->getContentSize().width / 2, _nave->getContentSize().height));
    addChild(misil);
    AudioEngine::play2d("Disparo.mp3");

    auto posicionFinal = Vec2(misil->getPositionX(), Director::getInstance()->getVisibleSize().height + misil->getContentSize().height);
    auto movimiento = MoveTo::create(1.0f, posicionFinal);
    auto eliminar = RemoveSelf::create();

    misil->runAction(Sequence::create(movimiento, eliminar, nullptr));
}

// Controles de movimiento
void Salvar_a_la_Tierra::inicializarMovimiento()
{
    auto escuchador = EventListenerKeyboard::create();

    escuchador->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        teclasPresionadas[keyCode] = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
            escape_tecla();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
            lanzarMisil();
        }
        };

    escuchador->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        teclasPresionadas[keyCode] = false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(escuchador, this);
}

// Metodo de actualización continua
void Salvar_a_la_Tierra::update(float delta)
{
    Vec2 movement;

    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_W]) movement.y += 5;
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_A]) movement.x -= 5;
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_S]) movement.y -= 5;
    if (teclasPresionadas[EventKeyboard::KeyCode::KEY_D]) movement.x += 5;

    Vec2 nuevaPosicion = _nave->getPosition() + movement;

    float maxX = Director::getInstance()->getVisibleSize().width - _nave->getContentSize().width;
    float maxY = Director::getInstance()->getVisibleSize().height - _nave->getContentSize().height;

    nuevaPosicion.x = std::max(0.0f, std::min(nuevaPosicion.x, maxX));
    nuevaPosicion.y = std::max(0.0f, std::min(nuevaPosicion.y, maxY));

    _nave->setPosition(nuevaPosicion);

    verificarColisiones();
    verificarColisionConNave();
    verificarColisionesEntreEnemigos();
}

// Crear enemigos con su patron 
void Salvar_a_la_Tierra::crearEnemigos(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float separacionHorizontal = 120.0f;

    for (int i = 0; i < 3; i++)
    {
        auto enemigo = Sprite::create("NaveEnemigaVersion1.png");

        float posX = (visibleSize.width / 2) + ((i - 1) * separacionHorizontal);
        float posY = visibleSize.height - enemigo->getContentSize().height / 2;
        enemigo->setPosition(Vec2(posX, posY));
        addChild(enemigo, 2);
        enemigos.push_back(enemigo);

        // Patron de movimiento
        auto moverDerecha = MoveTo::create(2.0f, Vec2(posX + 50, enemigo->getPositionY() - 50));
        auto moverIzquierda = MoveTo::create(2.0f, Vec2(posX - 50, enemigo->getPositionY() - 50));
        auto formacion = Sequence::create(moverDerecha, moverIzquierda, nullptr);

        enemigo->runAction(RepeatForever::create(formacion));

        // Disparar misiles
        auto dispararMisil = CallFunc::create([this, enemigo]() {
            lanzarMisilEnemigo(enemigo);
            });
        enemigo->runAction(RepeatForever::create(Sequence::create(dispararMisil, DelayTime::create(1.0f), nullptr)));
    }
}

// Disparar un misil enemigo
void Salvar_a_la_Tierra::lanzarMisilEnemigo(cocos2d::Sprite* enemigo)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto misil = Sprite::create("MisilEnemigo.png");
    misil->setName("MisilEnemigo");

    misil->setPosition(Vec2(enemigo->getPositionX(), enemigo->getPositionY() - enemigo->getContentSize().height / 2));
    addChild(misil, 1);

    auto moverAbajo = MoveTo::create(5.0f, Vec2(misil->getPositionX(), -misil->getContentSize().height));
    auto eliminarMisil = RemoveSelf::create();
    misil->runAction(Sequence::create(moverAbajo, eliminarMisil, nullptr));
}

// Verificar colisiones entre enemigos y misiles
void Salvar_a_la_Tierra::verificarColisiones()
{
    std::vector<Sprite*> misilesAEliminar;

    for (int i = enemigos.size() - 1; i >= 0; --i)
    {
        auto enemigo = enemigos[i];

        auto hijos = getChildren();
        for (auto hijo : hijos)
        {
            if (auto misil = dynamic_cast<Sprite*>(hijo))
            {
                if (misil->getName() == "Misil" && misil->getBoundingBox().intersectsRect(enemigo->getBoundingBox()))
                {
                    auto explosion = Sprite::create("explosion.png");
                    explosion->setPosition(enemigo->getPosition());
                    addChild(explosion, 3);
                    AudioEngine::play2d("explosion.mp3");

                    explosion->runAction(Sequence::create(DelayTime::create(0.2f), RemoveSelf::create(), nullptr));

                    enemigos.erase(enemigos.begin() + i);
                    enemigo->removeFromParent();
                    misilesAEliminar.push_back(misil);
                    actualizarScore(100);

                    break;
                }
            }
        }
    }

    for (auto misil : misilesAEliminar)
    {
        if (misil->getParent() != nullptr)
        {
            misil->removeFromParent();
        }
    }
    verificarColisionBoss1();
    verificarColisionBoss2();
    verificarColisionBoss3();
}
// Verificar colisiones entre enemigos (entre ellos)
void Salvar_a_la_Tierra::verificarColisionesEntreEnemigos() {
    for (size_t i = 0; i < enemigos.size(); ++i) {
        for (size_t j = i + 1; j < enemigos.size(); ++j) {
            // Verificar si los bounding boxes de los enemigos se intersectan
            if (enemigos[i]->getBoundingBox().intersectsRect(enemigos[j]->getBoundingBox())) {
                
                Vec2 posicionI = enemigos[i]->getPosition();
                Vec2 posicionJ = enemigos[j]->getPosition();

               
                Vec2 direccionI = (posicionI - posicionJ).getNormalized();
                Vec2 direccionJ = (posicionJ - posicionI).getNormalized();

                
                enemigos[i]->setPosition(posicionI + direccionI * 10); // Ajusta la distancia según sea necesario
                enemigos[j]->setPosition(posicionJ + direccionJ * 10);
            }
        }
    }
}
// 3 vidas para empezar 
void Salvar_a_la_Tierra::inicializarVidas()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    for (int i = 0; i < 3; ++i)
    {
        auto vidaSprite = Sprite::create("Nave.png");
        vidaSprite->setAnchorPoint(Vec2(0, 0));
        vidaSprite->setPosition(Vec2(10 + i * (vidaSprite->getContentSize().width + 5), 10)); // Separación entre vidas
        addChild(vidaSprite, 3);
        vidasSprites.push_back(vidaSprite);
    }
}
// Metodo para el impacto contra la nave 
void Salvar_a_la_Tierra::verificarColisionConNave()
{
    auto hijos = getChildren(); // Obtener todos los nodos hijos de la escena
    std::vector<Sprite*> misilesAEliminar;

    for (auto hijo : hijos)
    {
        if (auto misil = dynamic_cast<Sprite*>(hijo)) // Verificar si el nodo es un Sprite
        {
            if (misil->getName() == "MisilEnemigo" && misil->getBoundingBox().intersectsRect(_nave->getBoundingBox()))
            {
                misilesAEliminar.push_back(misil); // Agregar misil para eliminar
                perderVida(); // Reducir una vida
                AudioEngine::play2d("hit.mp3");
                break;
            }
        }
    }

    for (auto misil : misilesAEliminar) // Eliminar misiles que impactaron
    {
        if (misil->getParent() != nullptr)
        {
            misil->removeFromParent();
        }
    }
}

void Salvar_a_la_Tierra::perderVida()
{
    if (!vidasSprites.empty()) 
    {
        auto vidaSprite = vidasSprites.back(); 
        vidaSprite->removeFromParent(); 
        vidasSprites.pop_back(); 
    }

    if (vidasSprites.empty()) 
    {
        gameOver(); // Finalizar el juego
    }
}

void Salvar_a_la_Tierra::gameOver()
{
    this->getScheduler()->pauseTarget(this);
    AudioEngine::stop(audioFondo);

    // Detener las acciones de las naves enemigas
    for (auto enemigo : enemigos)
    {
        if (enemigo)
        {
            enemigo->stopAllActions(); // Detener todas las acciones del enemigo
        }
    }

    // Detener las acciones de los misiles enemigos
    auto hijos = this->getChildren();
    for (auto hijo : hijos)
    {
        if (hijo->getName() == "MisilEnemigo")
        {
            hijo->stopAllActions(); // Detener las acciones del misil enemigo
        }
    }
    //Director::getInstance()->pause(); // Pausar el juego
    // Crear botones
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();



    // Agregar el puntaje al leaderboard
    Leaderboard leaderboard;
    leaderboard.addScore(score); 

    
    leaderboard.saveScoresToFile();

  
    auto gameOverLayer = Layer::create();
    this->addChild(gameOverLayer, 10);
    // Crear un mensaje de "Game Over"
    auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 48);
    gameOverLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 200)); 
    addChild(gameOverLabel, 10); 



    // regresar al menú principal
    auto menuButton = MenuItemLabel::create(
        Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 36),
        [](Ref* sender) {
            auto menuScene = HelloWorld::createScene(); // Reemplaza con la escena de tu menú principal
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, menuScene));
        }
    );
    menuButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2+80));

    //jugar otra vez
    auto replayButton = MenuItemLabel::create(
        Label::createWithTTF("Play Again", "fonts/Marker Felt.ttf", 36),
        [](Ref* sender) {
            auto gameScene = Salvar_a_la_Tierra::createScene(); // Reiniciar la misma escena
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, gameScene));
        }
    );
    replayButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 30));

    //ir al leaderboard
    auto leaderboardButton = MenuItemLabel::create(
        Label::createWithTTF("Leaderboard", "fonts/Marker Felt.ttf", 36),
        [](Ref* sender) {
            auto leaderboardScene = Leaderboard::createScene(); // Ir a la escena del leaderboard
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, leaderboardScene));
        }
    );
    leaderboardButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 120));

    // añadir los botones
    auto menu = Menu::create(menuButton, replayButton, leaderboardButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 10);
}

void Salvar_a_la_Tierra::actualizarScore(int puntos)
{
    score += puntos; // Sumar puntos al puntaje
    scoreLabel->setString("Score: " + std::to_string(score)); // Actualizar la etiqueta
}

void Salvar_a_la_Tierra::escape_tecla() {
    Director::getInstance()->pause();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto btnContinuar = MenuItemImage::create(
        "btnContinuar.png",  // Imagen del estado normal
        "btnContinuarS.png", // Imagen del estado seleccionado
        CC_CALLBACK_1(Salvar_a_la_Tierra::Pausa, this)); 

    
    if (btnContinuar == nullptr ||
        btnContinuar->getContentSize().width <= 0 ||
        btnContinuar->getContentSize().height <= 0)
    {
        printf("Error while loading 'btnContinuar.png' and 'btnContinuarS.png'\n");
    }
    else {
       
        float x = visibleSize.width / 2;
        float y = visibleSize.height / 2+70;
        btnContinuar->setPosition(Vec2(x, y));
    }

    auto btnMenu = MenuItemImage::create(
        "MenuNN.png",  
        "MenuSS.png", 
        CC_CALLBACK_1(Salvar_a_la_Tierra::GoToMenu, this));  

   
    if (btnMenu == nullptr ||
        btnMenu->getContentSize().width <= 0 ||
        btnMenu->getContentSize().height <= 0)
    {
        printf("Error while loading 'MenuNN.png' and 'MenuSS.png'\n");
    }
    else {
       
        float x = visibleSize.width / 2;
        float y = visibleSize.height / 2 ;
        btnMenu->setPosition(Vec2(x, y));
    }

   
    auto btnVolumenMas = MenuItemLabel::create(
        Label::createWithTTF("+", "fonts/Marker Felt.ttf", 36),
        [&](Ref* sender) {
            float currentVolume = AudioEngine::getVolume(audioFondo);
            currentVolume = std::min(1.0f, currentVolume + 0.1f); 
            AudioEngine::setVolume(audioFondo, currentVolume);
        }
    );
    btnVolumenMas->setPosition(Vec2(visibleSize.width / 2 + 50, visibleSize.height / 2 - 150));

    
    auto btnVolumenMenos = MenuItemLabel::create(
        Label::createWithTTF("-", "fonts/Marker Felt.ttf", 36),
        [&](Ref* sender) {
            float currentVolume = AudioEngine::getVolume(audioFondo);
            currentVolume = std::max(0.0f, currentVolume - 0.1f); 
            AudioEngine::setVolume(audioFondo, currentVolume);
        }
    );
    btnVolumenMenos->setPosition(Vec2(visibleSize.width / 2 - 50, visibleSize.height / 2 - 150));

    auto menu = Menu::create(btnContinuar, btnMenu, btnVolumenMas, btnVolumenMenos, nullptr);
    menu->setPosition(Vec2::ZERO);
    menu->setName("pauseMenu");
    this->addChild(menu, 3);

    auto volumen = Sprite::create("volumen.png");
    volumen->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2-90));
    volumen->setName("volumenLabel");
    this->addChild(volumen, 3);

    

}

//------------------------------------------------------------------------------------------------Boss 1----------------------------------------------------------------------------------------
void Salvar_a_la_Tierra::crearBoss(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    boss = Sprite::create("boss1.png");
    boss->setName("Boss1");
    boss->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - boss->getContentSize().height));
    addChild(boss, 3);

    bossVida = 10; // Vida inicial del boss

    // Movimiento del boss
    auto moverDerecha = MoveTo::create(1.0f, Vec2(visibleSize.width - boss->getContentSize().width / 2, boss->getPositionY()));
    auto moverIzquierda = MoveTo::create(1.0f, Vec2(boss->getContentSize().width / 2, boss->getPositionY()));
    boss->runAction(RepeatForever::create(Sequence::create(moverDerecha, moverIzquierda, nullptr)));

    // Disparar misiles
    schedule(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::dispararMisilBoss), 0.5f); // Dispara cada 0.5 segundos
}

void Salvar_a_la_Tierra::dispararMisilBoss(float dt)
{
    if (boss == nullptr) return; // Salir de la función si el boss ya no existe
    auto misil = Sprite::create("MisilEnemigo.png");
    misil->setName("MisilEnemigo");

    misil->setPosition(Vec2(boss->getPositionX(), boss->getPositionY() - boss->getContentSize().height / 2));
    addChild(misil, 1);

    auto moverAbajo = MoveTo::create(2.0f, Vec2(misil->getPositionX(), -misil->getContentSize().height));
    auto eliminarMisil = RemoveSelf::create();
    misil->runAction(Sequence::create(moverAbajo, eliminarMisil, nullptr));
}

void Salvar_a_la_Tierra::verificarColisionBoss1()
{
    if (boss == nullptr || boss->getName() != "Boss1") return;

    std::vector<Sprite*> misilesAEliminar; // Lista de misiles que deben eliminarse
   
    for (auto hijo : getChildren())
    {
        if (auto misil = dynamic_cast<Sprite*>(hijo))
        {
            // Verificar si el misil golpea al boss
            if (misil->getName() == "Misil" && misil->getBoundingBox().intersectsRect(boss->getBoundingBox()))
            {
             

                bossVida--; 

                
         
                AudioEngine::play2d("explosion.mp3");
                misilesAEliminar.push_back(misil); 

                // Verificar si el boss ha sido derrotado
                if (bossVida <= 0)
                {
                    boss->removeFromParent(); 
                    boss = nullptr;         
                    actualizarScore(1000); 
                    // Añadir una vida si el jugador tiene menos de 3 vidas
                    if (vidasSprites.size() < 3)
                    {
                        auto nuevaVida = Sprite::create("Nave.png"); 
                        nuevaVida->setAnchorPoint(Vec2(0, 0));
                        nuevaVida->setPosition(Vec2(10 + vidasSprites.size() * (nuevaVida->getContentSize().width + 5), 10));
                        addChild(nuevaVida, 3);
                        vidasSprites.push_back(nuevaVida);
                    }
                    if (fondoActual != nullptr)
                    {
                        fondoActual->removeFromParent(); 
                    }
                    fondoActual = Sprite::create("fondodia.png"); // Cambiar al nuevo fondo
                    fondoActual->setAnchorPoint(Vec2(0, 0));
                    fondoActual->setPosition(Vec2(0, 0));
                    addChild(fondoActual, 0); // Agregar el nuevo fondo
                    break; 
                }
            }
        }
    }

    // Eliminar todos los misiles que impactaron al boss
    for (auto misil : misilesAEliminar)
    {
        if (misil->getParent() != nullptr)
        {
            misil->removeFromParent();
        }
    }
}

//------------------------------------------------------------------------------------------------Boss 2----------------------------------------------------------------------------------------

void Salvar_a_la_Tierra::crearBoss2(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    boss = Sprite::create("boss2.png");
    boss->setName("Boss2");
    boss->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - boss->getContentSize().height));
    addChild(boss, 3);

    bossVida = 15; 

    
    auto moveDownLeft = MoveTo::create(1.0f, Vec2(boss->getContentSize().width / 2, visibleSize.height / 2)); 
    auto moveUpLeft = MoveTo::create(1.0f, Vec2(boss->getContentSize().width / 2, visibleSize.height - boss->getContentSize().height)); 
    auto moveDownRight = MoveTo::create(1.0f, Vec2(visibleSize.width - boss->getContentSize().width / 2, visibleSize.height / 2)); 
    auto moveUpRight = MoveTo::create(1.0f, Vec2(visibleSize.width - boss->getContentSize().width / 2, visibleSize.height - boss->getContentSize().height));

    
    auto moveSequence = Sequence::create(moveDownLeft, moveUpLeft, moveDownRight, moveUpRight, nullptr);
    boss->runAction(RepeatForever::create(moveSequence)); 

   
    schedule(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::dispararMisilBoss2), 0.3f); 
}

void Salvar_a_la_Tierra::dispararMisilBoss2(float dt)
{
    if (boss == nullptr) return; 

    auto misil = Sprite::create("MisilEnemigo.png");
    misil->setName("MisilEnemigo");

    
    misil->setPosition(Vec2(boss->getPositionX(), boss->getPositionY() - boss->getContentSize().height / 2));
    addChild(misil, 1);

    // Movimiento del misil hacia abajo
    auto moverAbajo = MoveTo::create(1.5f, Vec2(misil->getPositionX(), -misil->getContentSize().height)); 
    auto eliminarMisil = RemoveSelf::create();
    misil->runAction(Sequence::create(moverAbajo, eliminarMisil, nullptr));
}

void Salvar_a_la_Tierra::verificarColisionBoss2()
{
    if (boss == nullptr || boss->getName() != "Boss2") return;

    std::vector<Sprite*> misilesAEliminar; 

    for (auto hijo : getChildren())
    {
        if (auto misil = dynamic_cast<Sprite*>(hijo))
        {
           
            if (misil->getName() == "Misil" && misil->getBoundingBox().intersectsRect(boss->getBoundingBox()))
            {
                bossVida--;

            

              
                AudioEngine::play2d("explosion.mp3");
                misilesAEliminar.push_back(misil); 

               
                if (bossVida <= 0)
                {
                    boss->removeFromParent(); 
                    boss = nullptr;         
                    actualizarScore(1500);  
                   
                    if (vidasSprites.size() < 3)
                    {
                        auto nuevaVida = Sprite::create("Nave.png"); 
                        nuevaVida->setAnchorPoint(Vec2(0, 0));
                        nuevaVida->setPosition(Vec2(10 + vidasSprites.size() * (nuevaVida->getContentSize().width + 5), 10));
                        addChild(nuevaVida, 3);
                        vidasSprites.push_back(nuevaVida); 
                    }
                   
                    unschedule(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::dispararMisilBoss2));
                    if (fondoActual != nullptr)
                    {
                        fondoActual->removeFromParent();
                    }
                    fondoActual = Sprite::create("FondoSalvar.png"); // Cambiar al nuevo fondo
                    fondoActual->setAnchorPoint(Vec2(0, 0));
                    fondoActual->setPosition(Vec2(0, 0));
                    addChild(fondoActual, 0); // Agregar el nuevo fondo

                    break; 
                }
            }
        }
    }

    // Eliminar todos los misiles que impactaron al boss
    for (auto misil : misilesAEliminar)
    {
        if (misil->getParent() != nullptr)
        {
            misil->removeFromParent();
        }
    }
}


//------------------------------------------------------------------------------------------------Boss 3----------------------------------------------------------------------------------------
void Salvar_a_la_Tierra::crearBoss3(float dt)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();

    boss = Sprite::create("boss3.png");
    boss->setName("Boss3");
    boss->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - boss->getContentSize().height));
    addChild(boss, 3);

    bossVida = 40; 

    // Movimiento aleatorio
    auto randomMove = CallFunc::create([this, visibleSize]() {
        float randomX = random(boss->getContentSize().width / 2, visibleSize.width - boss->getContentSize().width / 2);
        float randomY = random(visibleSize.height / 2, visibleSize.height - boss->getContentSize().height);
        auto moveTo = MoveTo::create(1.0f, Vec2(randomX, randomY));
        boss->runAction(moveTo);
        });

    boss->runAction(RepeatForever::create(Sequence::create(randomMove, DelayTime::create(1.0f), nullptr))); // Cambia de posición cada segundo

   
    schedule(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::dispararMisilBoss3), 0.7f); 
}

void Salvar_a_la_Tierra::dispararMisilBoss3(float dt)
{
    if (boss == nullptr) return; 

    auto visibleSize = Director::getInstance()->getVisibleSize();

   
    for (int angle = -11; angle <=11; angle += 15) 
    {
        auto misil = Sprite::create("MisilEnemigo.png");
        misil->setName("MisilEnemigo");

       
        misil->setPosition(Vec2(boss->getPositionX(), boss->getPositionY() - boss->getContentSize().height / 2));
        addChild(misil, 1);

      
        float radians = CC_DEGREES_TO_RADIANS(angle);
        float velocityX = sin(radians) * 200; 

       
        auto moveTo = MoveTo::create(2.0f, Vec2(misil->getPositionX() + velocityX, -misil->getContentSize().height));
        auto eliminarMisil = RemoveSelf::create();
        misil->runAction(Sequence::create(moveTo, eliminarMisil, nullptr));
    }
}

void Salvar_a_la_Tierra::verificarColisionBoss3()
{
    if (boss == nullptr || boss->getName() != "Boss3") return;

    std::vector<Sprite*> misilesAEliminar; 

    for (auto hijo : getChildren())
    {
        if (auto misil = dynamic_cast<Sprite*>(hijo))
        {
           
            if (misil->getName() == "Misil" && misil->getBoundingBox().intersectsRect(boss->getBoundingBox()))
            {
                bossVida--;

             
                AudioEngine::play2d("explosion.mp3");
                misilesAEliminar.push_back(misil); 

                
                if (bossVida <= 0)
                {
                    boss->removeFromParent();
                    boss = nullptr;          
                    actualizarScore(2000);  
                    
                    if (vidasSprites.size() < 3)
                    {
                        auto nuevaVida = Sprite::create("Nave.png"); 
                        nuevaVida->setAnchorPoint(Vec2(0, 0));
                        nuevaVida->setPosition(Vec2(10 + vidasSprites.size() * (nuevaVida->getContentSize().width + 5), 10));
                        addChild(nuevaVida, 3);
                        vidasSprites.push_back(nuevaVida); 
                    }
                  
                    unschedule(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::dispararMisilBoss3));
                    if (fondoActual != nullptr)
                    {
                        fondoActual->removeFromParent(); 
                    }
                    fondoActual = Sprite::create("fondodia.png"); 
                    fondoActual->setAnchorPoint(Vec2(0, 0));
                    fondoActual->setPosition(Vec2(0, 0));
                    addChild(fondoActual, 0); 

                    break; 
                }
            }
        }
    }

    // Eliminar todos los misiles que impactaron al boss
    for (auto misil : misilesAEliminar)
    {
        if (misil->getParent() != nullptr)
        {
            misil->removeFromParent();
        }
    }
}



//------------------------------------------------------------------------------------------------Funcionalidad---------------------------------------------------------------------------------
void Salvar_a_la_Tierra::juego(float dt) {
    static int iteracion = 0; // Contador global para ciclos (bosses y oleadas)
    static int oleada = 0;    // Contador local para oleadas de enemigos

    if (oleada < 10) {
        // Crear una oleada de enemigos
        crearEnemigos(0);
        oleada++;

       
        scheduleOnce(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::juego), 5.0f);
    }
    else {
        float tiempoParaBoss = 0.0f;

        // Programar el boss correspondiente
        if (iteracion % 3 == 0) {
            crearBoss(0);
            tiempoParaBoss = 15.0f; // 15 segundos para derrotar al Boss 1
        }
        else if (iteracion % 3 == 1) {
            crearBoss2(0); 
            tiempoParaBoss = 20.0f; // 20 segundos para derrotar al Boss 2
        }
        else {
            crearBoss3(0); 
            tiempoParaBoss = 40.0f; // 40 segundos para derrotar al Boss 3
        }

        iteracion++;
        oleada = 0; 

       
        scheduleOnce(CC_SCHEDULE_SELECTOR(Salvar_a_la_Tierra::juego), tiempoParaBoss);
    }
}
