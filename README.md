# TP3Shoot

Developed with Unreal Engine 5

UEPROJ
.
└── Content
    ├── Externals (Contiens touts les fichiers de provenance externe (plugin, lib d'asstes, etc))
    │   └── [Exemple : Polygone War Pack, ...]
    └── YOUR_PROJECT_NAME (Contiens touts les assets de votre jeu)
        ├── Inputs (Contiens touts ce qui est relatif aux inputs du jeu)
        │   ├── Actions
        │   ├── Configs
        │   └── MappingContexts
        ├── Assets (Contiens toutes le ressource utilise à la création de votre jeu mais que n'ont pas de code BP)
        │   ├── Textures
        │   ├── Meshs
        │   ├── Animations
        │   │   ├── Sequences
        │   │   ├── Montages
        │   │   └── ...
        │   └── ...
        ├── Levels (Contiens les niveaux de votre jeu)
        │   └── GYMS (niveaux de tests qui ne serront pas build dans le jeu)
        ├── Sounds (Contiens touts ce qui est relatif aux son de votre jeu)
        │   ├── Chanels
        │   ├── SoundsMixers
        │   ├── SoundsCues
        │   └── SoundsWave
        └── Blueprints  (doit suivre l'architecture des dossiers c++)
            ├── Characters
            ├── Controllers
            │   ├── AIControllers
            │   └── PlayerControllers
            ├── GameModes
            ├── GameStates
            ├── PlayerStates
            ├── UI
            │   ├── HUD
            │   ├── Widgets
            │   └── Components
            ├── GameInstances
            ├── SaveGames
            └── ...
                └── ...
