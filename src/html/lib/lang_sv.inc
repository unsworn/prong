<?php

// Generic strings
define("STR_APP_NAME", "p|r|o|n|g");
define("STR_LOGIN_TITLE", "Inlogging");
define("STR_LOGIN_USER", "Användare");
define("STR_LOGIN_PASS", "Lösenord");

// Main menu (index.php)
define("STR_MAIN_MENU_UPLOAD", "Skapa spel");
define("STR_MAIN_MENU_BROWSE", "Spela");                  
define("STR_MAIN_MENU_EDIT", "Redigera");

// Navigation (navigation.inc)
define("STR_NAV_PRONG", "[prong]");
define("STR_NAV_QUIT", "Logga ut");
define("STR_NAV_HELP", "Hjälp");

// Upload dialog (upload.php);
define("STR_UPLOAD_HEADER", "Skapa spel");
define("STR_UPLOAD_VALIDATE", "Ingen fil är vald");
define("STR_UPLOAD_HR1", "Skanna in mallen");
define("STR_UPLOAD_HB1", "Filen skall vara av typ JPEG.<br>Tänk på att upplösningen för ett A3 ska vara runt 100dpi och ungefär 200dpi för ett A4");

define("STR_UPLOAD_HR2", "Välj fil att ladda upp");
define("STR_UPLOAD_HB2", "Använd knappen för att hämta den inskannade bilden från hårddisken");

define("STR_UPLOAD_CHOOSE", "Välj fil");
define("STR_UPLOAD_SUBMIT", "Importera");

// Adjust dialog (adjust.php)
define("STR_ADJUST_SUBMIT", "Beskär");
define("STR_ADJUST_SKIP", "Nästa");
define("STR_ADJUST_HELP", "Hej.<br> Det du ser är den inskannade bilden som nu skall beskäras.<p><b>Om bilden redan är beskuren visas knappen \"Nästa\" så du kan gå direkt till spelredigering.</b><p>En rosa rektangel visar hur bilden kommer att beskäras, i varje hörn visas även en förstoring av rektangelns linjer så att du lätt kan passa in den mot skärmärken på bilden. Du kan flytta, skala och rotera, de inre svagt blå ytorna visa var i förhållande till den större rektangeln alla element i spelet befinner sig, så som bakground, huvudperson etc.<p>Så här beskär du<p> Om dokumentet inte är helt rakt använd roterings pilarna som omger positionerings korset i mitten tills du tycker att dokumentet är tillräckligt rakt i förhållande till rektangeln.<p> Positionera rektangeln med hjälp av korset i mitten så att övre vänstra hörn linjerar med skärmärket.<p> Justera sedan rektangelns bredd med pilen till höger som pekar in mot dokumentets mitt.<p> Justera nu rektangelns höjd med pilen nertill.<br>Är du nöjd, klicka på beskär");

// Import dialog (import.php, import.js)
define("STR_IMPORT_HEADER", "Skapa bana");
define("STR_IMPORT_SUBMIT", "Skapa");
define("STR_IMPORT_ABORT" , "Avbryt");

define("STR_IMPORT_GAME_NAME", "Spelets namn");
define("STR_IMPORT_GAME_DEFAULT", "Klicka här");
define("STR_IMPORT_GAME_GOAL", "Mål");
define("STR_IMPORT_SPRITE_TITLE", "Spelare");
define("STR_IMPORT_PROPERTIES", "Egenskaper");

define("STR_GAME_PLAYING", "Spelar");
define("STR_GAME_DONE_TITLE", "Hyves laxar!");
define("STR_GAME_DONE_UNDER", "Du klarade dig till målet");
define("STR_GAME_DONE_BODY", "Nu kan du antingen spela igen, eller gå vidare till nästa spel");
define("STR_GAME_PLAY_AGAIN", "Spela igen");
define("STR_GAME_PLAY_RANDOM", "Nästa spel");

define("STR_GAME_FAIL_TITLE", "Ajdå");
define("STR_GAME_FAIL_UNDER", "Du ramlade av banan");
define("STR_GAME_FAIL_BODY", "Firrar är hala");


// Play
define("STR_BROWSE_PLAY", "Spela");

// Edit
define("STR_BROWSE_EDIT", "Redigera");

define("STR_BROWSE_TITLE", "Visar spel för");

// Help
define("STR_HELP_HEADER", "Hur funkar det ?");
define("STR_HELP_BACK"  , "Tillbaka");

define("STR_DONE_HEADER", "Grattis");
define("STR_DONE_BODY", "Du har nu klarat alla spelen");

$templateNames = array(
           "main"        => "Hjälte",
           "npone"       => "Kumpan 1",
           "nptwo"       => "Kumpan 2",
           "lowgravity"  => "Flygvärld",
           "friction"    => "Vattenvärld",
           "normal"      => "Normalvärld",
           "highgravity" => "Sirapsvärld",
           "follow"      => "Följer mig",
           "hide"        => "Gömmer sig",
           "help"        => "Hjälper mig",
           "block"       => "Stoppar mig",
           "push"        => "Knuffar mig",
           "cloak"       => "Skrämmer mig",
           "normalsized" => "Normal storlek",
           "mediumsized" => "Liten storlek",
           "microsized"  => "Fjutte storlek");

?>