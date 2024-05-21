﻿# SAE_23_RESEAU

## Naming conventions in C

- All macros and constants in caps: **MAX_BUFFER_SIZE**, **TRACKING_ID_PREFIX**.
- Struct names and typedef's in camelcase: **GtkWidget**, **TrackingOrder**.
- Functions that operate on structs: classic C style: **gtk_widget_show()**, **tracking_order_process()**.
- Pointers: nothing fancy here: **GtkWidget \*foo**, **TrackingOrder \*bar**.
- Functions that are there, but shouldn't be called directly, or have obscure uses, or whatever: one or more - underscores at the beginning: **\_refrobnicate_data_tables()**, **\_destroy_cache()**.

## To Compile and Execute:
` make | ./bin/main `