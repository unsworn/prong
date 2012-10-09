<?php

include("config.inc");
include("auth.inc");
include("utils.inc");
include("deskew.inc");

if ($curUser->id == 4)
	header("Location: 403.html");

$preview = NULL;

if (isset($_FILES["file"]))
{
    $uploadPath = getUniqueDir(FL_GAME_DIR);

    if ($uploadPath == NULL)
        die("Error, disk catalog full");
    
    $filename = $_FILES['file']['name'];
    $filesize = $_FILES['file']['size'];

    $parts = explode(".", $filename);
    $filetype = end($parts);

    mkdir($uploadPath);
    
    $outfile = $uploadPath . "/uploaded.jpg";
    $inffile = $uploadPath . "/info.js";
    $preview = $uploadPath . "/" . FL_PREVIEW_NAME;
    
    if (!copy($_FILES['file']['tmp_name'], $outfile))
        die("Error, unable to receive upload");

    $op = new ImageOp($outfile);

    $op->scaleW(FL_PREVIEW_SIZE, $preview);

    $id = getDirId($uploadPath);

    $op->write($inffile);
    
    header("Location: adjust.php?id=" . $id);
    
}

   
?>

<html>    
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />
<script type="text/javascript" src="http://code.jquery.com/jquery-latest.js"></script>
<script type="text/javascript">  
    function submitForm()
    {
        var fform = $("#uploadform").get(0);
        var ffile = fform.file.value;

        if (ffile.length == 0)
        {
            alert("<?=STR_UPLOAD_VALIDATE?>");
            return ;
        }
        fform.submit();
    }
</script>

<link rel="stylesheet" type="text/css" href="css/prong.css" />
<title><?=STR_APP_NAME?></title>
</head>
<body>
<center>                      	
  <?include("navigation.inc");?>
  <div class="header">
    <?=STR_UPLOAD_HEADER?>
  </div>
  <br>
  <div id="upload_box">
    <table border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td valign="top">
          <h5>1. <?=STR_UPLOAD_HR1?></h5>
          <span class="upload_help">
          <?=STR_UPLOAD_HB1?><br>
          </span>
          <h5>2. <?=STR_UPLOAD_HR2?></h5>
          <span class="upload_help">
          <?=STR_UPLOAD_HB2?><br>
          </span>
          <br>
          <div class="filechooser">
          <form name="upload" id="uploadform" method="post" action="<?=$_SERVER['PHP_SELF'];?>" enctype="multipart/form-data">
            <input type="file" name="file" id="file">
          </form>
          </div>
	    </td>
        <td valign="top">
          <a href="#" class="button black" onClick="submitForm();"><?=STR_UPLOAD_SUBMIT?></a>
        </td>
      </tr>
    </table>					
  </div>
</center>
</body>

</html>
