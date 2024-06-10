# SAE_23_RESEAU

## Naming conventions in C

- All macros and constants in caps: **MAX_BUFFER_SIZE**, **TRACKING_ID_PREFIX**.
- Struct names and typedef's in camelcase: **GtkWidget**, **TrackingOrder**.
- Functions that operate on structs: classic C style: **gtk_widget_show()**, **tracking_order_process()**.
- Pointers: nothing fancy here: **GtkWidget \*foo**, **TrackingOrder \*bar**.
- Functions that are there, but shouldn't be called directly, or have obscure uses, or whatever: one or more - underscores at the beginning: **\_refrobnicate_data_tables()**, **\_destroy_cache()**.

## To Compile and Execute:

`make | ./bin/main`

## What we accomplished:

- Vous trouverez dans le dossier diagram, le diagramme de structure de notre projet.
- On échange les données entre les différents équipements du réseau.
- On a implémenté le STP (Spanning Tree Protocol) pour éviter les boucles dans le réseau.
- On arrive à afficher des trames en mode utilisateur et en mode Hexadécimal.
