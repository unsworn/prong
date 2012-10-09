<?php
  
if (!defined("DB_NAME"))
{
    define("DB_NAME", "prong");
    define("DB_USER", "web");
    define("DB_PWWD", "finkelfantom");
    define("DB_HOST", "localhost");

    
    define("EM_TYPE_UNKNOWN"   , 0);
    define("EM_TYPE_BACKGROUND", 1);
    define("EM_TYPE_PLAYER"    , 2);
    define("EM_TYPE_STATIC"    , 3);
    define("EM_TYPE_ENTITY"    , 4);
    define("EM_TYPE_SPAWN"     , 5);

    define("CT_TYPE_ANY"        , 0);
    define("CT_TYPE_FLOOR"     , 1);
    define("CT_TYPE_WALL"      , 2);
    define("CT_TYPE_TRAP"      , 3);
    define("CT_TYPE_SPAWN"     , 4);
    define("CT_TYPE_EXIT"      , 5);

    define("FL_INSTALL_DIR"  , "/var/www/prong.uidev.se/htdocs");
    
    define("FL_TEMPLATE_NAME", "template.js");
    define("FL_DATA_DIR"     , "data");
    define("FL_GAME_DIR"     , "data/game");    
    define("FL_UPLOAD_NAME"  , "uploaded");
    define("FL_PREVIEW_NAME" , "payload.jpg");    
    define("FL_PREVIEW_SIZE" , 500);

    define("GAME_WIDTH",  600);
    define("GAME_HEIGHT", 400);

    define("EDIT_WIDTH",  800);
    define("EDIT_HEIGHT", 400);
    
}

?>
