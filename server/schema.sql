create database StrangerThings;
use StrangerThings;

create table singleQueue
(
queueKey varchar(255) not null,
queueValue varchar(255) not null,
primary key (queueKey)
);

insert into singleQueue values ('strangerThings','');

create user 'stUser'@'localhost' identified by 'strangerthings';
grant all privileges on * . * to 'stUser'@'localhost';
flush privileges;