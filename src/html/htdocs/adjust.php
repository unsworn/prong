<?php

include("config.inc");
include("utils.inc");
include("auth.inc");	
include("deskew.inc");

if ($curUser->id == 4)
	header("Location: 403.html");

if (!isset($_REQUEST["id"]))
    die("Error, adjust requires an id");

$newUpload = true;

$gameId   = $_REQUEST["id"];
$gamePath = FL_GAME_DIR . "/" . $gameId;

$previewFile = $gamePath . "/" . FL_PREVIEW_NAME;
$uploadFile  = $gamePath . "/" . FL_UPLOAD_NAME . ".jpg";

if (file_exists($gamePath . "/adjusted.js"))
    $newUpload = false;

if (isset($_REQUEST["save_game"]))
{
        
    if ($gamePath == NULL)
        die("Error, disk catalog full");

    if (!file_exists($gamePath))
        mkdir($gamePath);

    {
        $val = array(
            "game"  => $gameId,
            "angle" => $_REQUEST["save_angle"],
            "cropx" => $_REQUEST["save_cropx"],
            "cropy" => $_REQUEST["save_cropy"],
            "cropw" => $_REQUEST["save_cropw"],
            "croph" => $_REQUEST["save_croph"],
            "boxx"  => $_REQUEST["save_boxx"],
            "boxy"  => $_REQUEST["save_boxy"],
            "boxw"  => $_REQUEST["save_boxw"],
            "boxh"  => $_REQUEST["save_boxh"]);
        
        $data = json_encode($val);

        $apath = $gamePath . "/adjusted.js";

        file_put_contents($apath, $data);

        $crop = array($val["cropx"], $val["cropy"], $val["cropw"], $val["croph"]);

        $tmpl = FL_INSTALL_DIR . "/" . FL_TEMPLATE_NAME;

        $op = new ImageOp($uploadFile);
    
        $op->process(abs($val["angle"]), GAME_HEIGHT, $crop, $tmpl, $gamePath);

        header("Location: import.php?id=$gameId");
        
    }
    
}

?>

<html>    
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />

<script type="text/javascript">
    var gUploadId    = "<?=$_REQUEST['id'];?>";
    var gPreviewPath = "<?=$gamePath;?>";
    var gPreviewFile = "<?=$previewFile;?>";
</script>

<script type="text/javascript" src="http://code.jquery.com/jquery-latest.js"></script>
<script type="text/javascript" src="js/processing.js"></script>
<script type="text/javascript" src="js/common.js"></script>
<script type="text/javascript" src="js/widget.js"></script>        
<script type="text/javascript" src="js/adjust.js"></script>    

<script type="text/javascript">
    function submitForm()
    {
        var fform = $("#save_form").get(0);
        if (!fform)
            alert("Error");
        fform.submit();
    }
</script>

<link rel="stylesheet" type="text/css" href="css/prong.css" />
    
<title><?=STR_APP_NAME?></title>
</head>
<body>
<center>

<?include("navigation.inc");?>

<div id="adjust_content">
<table border="0" width="800">
  <tr>
    <td width="600">
        <div class="ajdust_main">
          <canvas id="adjust_canvas">
        </canvas>
        </div>
    </td>
    <td valign="top" width="200">
        <form name="form" id="save_form" method="post" action="adjust.php">
        <input type="hidden" name="id" value="<?=$_REQUEST['id'];?>">
        <input type="hidden" name="save_game" value="true">
        <input type="hidden" name="save_angle" value="0">
        <input type="hidden" name="save_cropx" value="0">
        <input type="hidden" name="save_cropy" value="0">
        <input type="hidden" name="save_cropw" value="0">
        <input type="hidden" name="save_croph" value="0">        
        <input type="hidden" name="save_boxx" value="0">
        <input type="hidden" name="save_boxy" value="0">
        <input type="hidden" name="save_boxw" value="0">
        <input type="hidden" name="save_boxh" value="0">        
        </form>

        <table border="0" width="100%" cellpadding="0" cellspacing="0">
        <tr>
        <?php
           if ($newUpload)
           {
               echo "<td>";
               echo "<a href='#' onClick='submitForm();' class='button black'>" . STR_ADJUST_SUBMIT . "</a>";
               echo "</td>";
           }
           else
           {
               echo "<td>";
               echo "<a href='#' onClick='submitForm();' class='button gray'>" . STR_ADJUST_SUBMIT . "</a>";               
               echo "</td></tr><tr><td height='4'></td></tr><tr><td>";
               echo "<a href='import.php?id=$gameId' class='button black'>" . STR_ADJUST_SKIP . "</a>";
               echo "</td>";
           }
        ?>
        </tr>
        </table>
        <span class="adjust_help">
        <p>

        <?=STR_ADJUST_HELP?>

        </span>
    </td>
  </tr>
</table>
</div>
</center>
</body>
</html>
