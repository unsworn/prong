# drop old nasty stuff

# users
drop table if exists user;

# groups
drop table if exists group;

# game
drop table if exists game;

# constraints
drop table if exists constraints;

# properties
drop table if exists properties;

# element
drop table if exists element;


# user table

create table users (
    id int(32) not null auto_increment,
    gid int(32) not null default '0',
    name varchar(255) not null,
    mail varchar(255) not null,        
    secret varchar(255) not null,
    approved int(1) not null default '0',
    expires timestamp default 0,
    primary key (id)
);

create table groups (
    id int(32) not null auto_increment,
    name varchar(255) not null,
    primary key (id)
);

create table constraints (
    game int(64) not null default '0',
    type int(32) not null default '0',
    points longblob default null,
    bb_top int(32) not null default '0',
    bb_left int(32) not null default '0',
    bb_right int(32) not null default '0',
    bb_bottom int(32) not null default '0'
);

create table properties (
    element int(64) not null default '0',
    name varchar(255) not null default '',
    value varchar(512) default null
);

create table game (
    id int(64) not null auto_increment,    
    user int(32) not null,
    name varchar(255) not null,
    created timestamp not null,
    modified timestamp not null,    
    source varchar(255) default null,
    root varchar(512) default null,
    primary key (id)
);

create table element (
    id int(64) not null auto_increment,
    project int(64) not null,
    name varchar(255) not null,
    bb_top int(32) not null default '0',
    bb_left int(32) not null default '0',
    bb_right int(32) not null default '0',
    bb_bottom int(32) not null default '0',
    em_type int(32) not null default '0',    
    primary key (id)
);

