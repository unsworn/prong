<?php

include("config.inc");
include("utils.inc");
include("auth.inc");	
include("template.inc");
include("deskew.inc");

if ($curUser->id == 4)
	header("Location: 403.html");

if (!isset($_REQUEST["id"]))
    die("Error, adjust requires an id");

$gameId   = $_REQUEST["id"];

$gamePath = FL_GAME_DIR . "/" . $gameId;


?>

<html>    
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />

<script type="text/javascript">
    <?php
      gameid_js($gameId);
      preload_js($gameId);
      template_strings_js();
    ?>

    var exitHook;

    var str_lbl_name_title   = "<?=STR_IMPORT_GAME_NAME?>";
    var str_lbl_default      = "<?=STR_IMPORT_GAME_DEFAULT?>";
    var str_lbl_sprite_title = "<?=STR_IMPORT_SPRITE_TITLE?>";
    var str_lbl_properties   = "<?=STR_IMPORT_PROPERTIES?>";
    var str_lbl_goal         = "<?=STR_IMPORT_GAME_GOAL?>";

</script>

<script type="text/javascript" src="http://code.jquery.com/jquery-latest.js"></script>
<script type="text/javascript" src="js/processing.js"></script>
<script type="text/javascript" src="js/common.js"></script>
<script type="text/javascript" src="js/engine.js"></script>        
<script type="text/javascript" src="js/import.js"></script>    

<link rel="stylesheet" type="text/css" href="css/prong.css" />
    
<title><?=STR_APP_NAME?></title>
</head>
<body>
<center>

<?include("navigation.inc");?>
<div class="header">
    <?=STR_IMPORT_HEADER?>
</div>
<br>
<table border="0" width="800" cellpadding="0" cellspacing="0">
  <tr>
    <td width="600">
        <div class="import_main">
          <canvas id="import_canvas">
        </canvas>
        </div>
    </td>
  </tr>
  <tr>
    <td style="background: #282828;" align="right" height="30">
         <a href="index.php" class="button gray"><?=STR_IMPORT_ABORT?></a>
         <a href="#" class="button black" onClick="exitHook();"><?=STR_IMPORT_SUBMIT?></a>
    </td>
  </tr>
</table>
</center>
</body>
</html>



