create database StrangerThings;
use StrangerThings;

create table singleQueue
(
key varchar(255) not null,
value varchar(255) not null,
primary key (key)
);

create user 'stUser'@'localhost' identified by 'strangerthings';
grant all privileges on * . * to 'st'@'localhost';
flush privileges;