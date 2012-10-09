<?php                    

include ("user.inc");

session_start();  

$curUser = NULL;

if (isset($_REQUEST["logout"]))
{
	session_unset();
	session_destroy();
	$curUser = NULL;
}


if (empty($_SESSION["user"]) && !isset($allowAuthOverride))
{
    header("Location: login.php");
}
else
{
    if (array_key_exists("user", $_SESSION))
    {
        $curUser = $_SESSION["user"];
    }
    else if (isset($allowAuthOverride))
    {
        include("lang_sv.inc");
    }
    else
        header("Location: login.php");
    
    if (!defined("APP_NAME"))
    {

        if ($curUser != NULL)
        {
            switch($curUser->getLanguage()) 
            {
                case "sv":
                    include ("lang_sv.inc");
                    break;
                default:
                    include ("lang_sv.inc");
                    break;
            }
        }
        
    }
    
    
    
}



?>