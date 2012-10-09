<?php

$allowAuthOverride = true;

include("config.inc");
include("utils.inc");
include("auth.inc");	
include("template.inc");


if (!isset($_REQUEST["id"]))
    die("Can not load game without ID");

$hist         = "";
$tries        = 0;
$importPlayer = false;
$importSource = "";

if (isset($_REQUEST["last"]))
{
    $hist = $_REQUEST["last"];

    $last  = substr($hist, 0, 2);
    $first = substr($hist, -2);
    
    $importPlayer = true;
    $importSource = $first;
}

if (isset($_REQUEST["tries"]))
{
    $tries = $_REQUEST["tries"];
}

if ($_REQUEST["id"] == "rand" && $curUser != NULL)
{

    $db = Mysql::getConnection();

    $db->query("SELECT id FROM game WHERE grp = $curUser->gid;");

    $allowed   = array();

    while($o = $db->fetchObject())
    {
        array_push($allowed, $o->id);
    }

    
    $content   = scandir(FL_GAME_DIR);
    $filtered  = array();
    
    foreach($content as $entry)
    {
        if (preg_match("/[a-z]{2}/", $entry) && strstr($hist, $entry) === FALSE)
            if (file_exists(FL_GAME_DIR . "/" . $entry . "/main.png"))
                if (in_array($entry, $allowed))
                    array_push($filtered, $entry);
    }

    $len = count($filtered);

    if ($len == 0)
    {
        //header("Location: done.php");
        die();
    }

    
    $gameId = $filtered[rand(0, $len-1)];
    
}
else
    $gameId = $_REQUEST["id"];

if (empty($hist))
    $hist = $gameId;
?>

<html>    
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />    

<script type="text/javascript">

<?gameid_js($gameId);?>
var WIDTH   = <?=GAME_WIDTH?>;
var HEIGHT  = <?=GAME_HEIGHT?>;
var EDITOR  = {width: <?=EDIT_WIDTH?>, height: <?=EDIT_HEIGHT?>};
var IMPORT  = false;
var HISTORY = "<?=$hist?>";
var TRIES   = <?=$tries?>;

<?if($importPlayer) {
    echo "var gImportPlayer = true;\n";
    echo "var gImportSource = \"$importSource\";\n";
}else{
    echo "var gImportPlayer = false;\n";
}
?>

var gUser                = <?=($curUser != NULL) ? $curUser->id : 0;?>;

var str_game_done_title  = "<?=STR_GAME_DONE_TITLE?>";
var str_game_done_under  = "<?=STR_GAME_DONE_UNDER?>";
var str_game_done_body   = "<?=STR_GAME_DONE_BODY?>";
var str_game_play_again  = "<?=STR_GAME_PLAY_AGAIN?>";
var str_game_play_random = "<?=STR_GAME_PLAY_RANDOM?>";
var str_game_fail_title  = "<?=STR_GAME_FAIL_TITLE?>";
var str_game_fail_under  = "<?=STR_GAME_FAIL_UNDER?>";
var str_game_fail_body   = "<?=STR_GAME_FAIL_BODY?>";

</script>

<script type="text/javascript" src="http://code.jquery.com/jquery-latest.js"></script>
<script type="text/javascript" src="js/processing.js"></script>
<script type="text/javascript" src="js/common.js"></script>
    
<script type="text/javascript" src="js/sprite.js"></script>                    
<script type="text/javascript" src="js/collision.js"></script>
<script type="text/javascript" src="js/scrolling.js"></script>


<link rel="stylesheet" type="text/css" href="css/prong.css" />

<title><?=STR_APP_NAME?> <?=$gameId;?></title>
</head>
<body>
  <center>                      	

<? if($curUser != NULL) include("navigation.inc");?>        
    <div class="header">
        <b><span class="game_name" id="gname">name</span></b>
    </div>
    <br>
    <script type="text/javascript" src="js/game.js"></script>        
 </center>

 <!-- div class="wonka" id="gamewon">
        who
 </div -->

<?php
      loadSounds(FL_DATA_DIR . "/sounds/default");
?>

</body>

</html>
