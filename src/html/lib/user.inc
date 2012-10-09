<?php
include("db.inc");

class User
{                         
    private function __construct($ud, $gd)
    {           
        $this->id       = $ud["id"];
        $this->name     = $ud["name"];
        $this->gid      = $ud["gid"];
        $this->mail     = $ud["mail"];
        $this->approved = $ud["approved"];
        $this->expires  = $ud["expires"];
        $this->group    = $gd["name"];
    }
    
    public static function getUser($username, $secret)
    {
        
        $db = Mysql::getConnection();
        
        $db->query("SELECT * FROM users WHERE name = '$username' AND secret = PASSWORD('$secret') LIMIT 1");
                        
        if (!$db->hasResult())
            return NULL;
            
        $gd = NULL;
        $ud = $db->fetchAssoc();
        
        $db->free();
        
        $gid = $ud["gid"];
        
        if ($gid != 0)
        {
            if ($db->query("SELECT * FROM groups WHERE id = $gid LIMIT 1"))
            {
                $gd = $db->fetchAssoc();                
            }                           
            $db->free();            
        }               
        
        return new User($ud, $gd);
        
    }                

    public function getId()
    {
        return $this->id;
    }

    public function getGroup()
    {
        return $this->gid;
    }
    
    public function getLanguage()
    {
        return "sv";
    }
}

?>
