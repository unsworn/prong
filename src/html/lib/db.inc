<?php
include("config.inc");

class Mysql
{                                       
    private static $instance;
                        
    private $db;
    private $result;
    
    private function __construct()
    {
        $this->db = NULL;
        $this->result = NULL;
    }
    
    public static function getConnection()
    {
        if (!self::$instance)
        {
            self::$instance = new Mysql();
            self::$instance->connect();
        }
        
        return self::$instance;
        
    }                                   
    
    public function connect()
    {
        if (is_null($this->db))
        {
            $this->db = mysql_connect(DB_HOST, DB_USER, DB_PWWD)
                or die("Unable to connect to mysql database");
            mysql_select_db(DB_NAME, $this->db);
        }                                       
       
    }   
    
    public function close()
    {                    
        if (!is_null($this->result))
            $this->free();
        if (!is_null($this->db))
            mysql_close($this->db);
        $this->db = NULL;
    }   

    public function delete($qstr)
    {
        $this->exec($qstr);
    }
    public function insert($qstr)
    {
        $this->exec($qstr);
    }

    public function exec($qstr)
    {
        if (!is_null($this->result))
            $this->free();
        mysql_query($qstr);
    }
    
    public function query($qstr)
    {                       
        if (!is_null($this->result))
            $this->free();
        $this->result = mysql_query($qstr);
        return $this->hasResult();
    }                             
                                                        
    public function hasResult()
    {
        if (!is_null($this->result))
            return mysql_num_rows($this->result) > 0;
        return false;
    }                
    
    public function free()
    {                           
        if (!is_null($this->result))
            mysql_free_result($this->result);
        $this->result = NULL;
    }
                   
    public function fetchObject()
    {
        return mysql_fetch_object($this->result);
    }                                            
    
    public function fetchAssoc()
    {
        return mysql_fetch_assoc($this->result);
    }                                           
    
    public function fetchRow()
    {
        return mysql_fetch_row($this->result);        
    }

}