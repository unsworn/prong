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
    game varchar(32) not null default '',
    type varchar(32) not null default '',
    sx int(32) not null default 0,
    sy int(32) not null default 0,
    ex int(32) not null default 0,
    ey int(32) not null default 0
);

create table properties (
    game varchar(32) not null default '',
    element varchar(255) not null default '',
    name varchar(255) not null default '',
    enabled char(1) default '0'
);

create table game (
    id varchar(32) not null default '--',
    user int(32) not null,  
    grp int(32) not null,
    name varchar(255) not null,
    created timestamp not null,
    modified timestamp not null,    
    source varchar(255) default null,
    root varchar(512) default null,
    sounds varchar(512) default 'default',
    mtempo int(32) default 5,
    primary key (id)
);

create table element (
    game varchar(32) not null default '',
    name varchar(255) not null default '',
    sx int(32) not null default 0,
    sy int(32) not null default 0
);

