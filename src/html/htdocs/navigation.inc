<div id="navigation">
   <div class="nav_button" id="nav_home">
     <div class="nav_label">
     <a href="index.php"><?=STR_NAV_PRONG?></a>
     </div>     
  </div>
  <div class="nav_button" id="nav_quit">
     <div class="nav_label">
        <a href="<?php echo $_SERVER['PHP_SELF'];?>?logout=do"><?=STR_NAV_QUIT?></a>
     </div>
  </div>
  <div class="nav_button" id="nav_help">
     <div class="nav_label">
       <a href="help.php"><?=STR_NAV_HELP?></a>
     </div>
  </div>
</div>
</html>
