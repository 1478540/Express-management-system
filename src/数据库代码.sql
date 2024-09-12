show databases;

create database Express_Management_System;

use express_management_system;

create table addressee(
name varchar(10),
number varchar(15) primary key,
address varchar(20)
);

create table sender(
name varchar(10),
number varchar(15) primary key,
address varchar(20)
);

create table package(
tracking_number varchar(20) primary key,
courier_company varchar(15),
weight int,
zip_code varchar(20),
pickup_code varchar(20)
);

alter table package 
add exist bool
NOT NULL;

desc package;

create table SA(
sender_number varchar(15),
tracking_number varchar(20) primary key,
addressee_number varchar(15),
constraint sa_sender_foreign foreign key (sender_number) references sender(number) ON DELETE CASCADE ON UPDATE CASCADE,
constraint sa_addressee_foreign foreign key (addressee_number) references addressee(number) ON DELETE CASCADE ON UPDATE CASCADE
);

create table DetailedDate(
receipt_date date NOT NULL,
pickup_date date,
tracking_number varchar(20) primary key
);

show tables from express_management_system;

insert into addressee value('小明','111','1','12121');
insert into sender value('小红','222','2');
insert into SA value(222,'666',111);
insert into SA value(222,'777',111);
insert into package value('777','北京快递',32,'12212','8383',true);
insert into package value('222','东方快递',1112,'12212','8383',true);

SET SQL_SAFE_UPDATES=0;

delete from sender;
delete from sa;
delete from addressee;
delete from package;
delete from detaileddate;

update sender set number='333' where name='小红';

select * from addressee;
select * from sender;
select * from sa;
select * from package;
select * from detaileddate;


DELIMITER $
CREATE TRIGGER tr_sa_delete_sender
before delete 
ON sa 
FOR EACH ROW 
BEGIN  
    DECLARE returned_rows INT(11);

    SELECT count(sa.sender_number) INTO returned_rows FROM sa WHERE sa.sender_number=old.sender_number;
    IF returned_rows <= 1 THEN  
        delete from sender where sender.number=old.sender_number;
    END IF;
END;
$
DELIMITER ;
drop trigger tr_sa_delete_sender;


DELIMITER $
CREATE TRIGGER tr_package_insert_detaileddate
after insert 
ON  package
FOR EACH ROW 
BEGIN  
   insert into detaileddate value(curdate(),NULL,new.tracking_number);
END;
$
DELIMITER ;
drop trigger tr_package_insert_detaileddate;


DELIMITER $
CREATE TRIGGER tr_package_delete_detaileddate
before delete 
ON  package
FOR EACH ROW 
BEGIN  
   delete from detaileddate where detaileddate.tracking_number=old.tracking_number;
END;
$
DELIMITER ;
drop trigger tr_package_delete_detaileddate;


DELIMITER $
CREATE TRIGGER tr_package_update_detaileddate
after update 
ON  package
FOR EACH ROW 
BEGIN  
IF new.exist=false THEN  
update detaileddate set pickup_date=curdate() where tracking_number=new.tracking_number;
END IF;
END;                
$
DELIMITER ;
drop trigger tr_package_update_detaileddate;


#########################################################################


create view subview1 as
select courier_company,count(*) as '收件量' 
from 
(
select courier_company
from package,detaileddate 
where detaileddate.tracking_number=package.tracking_number 
and detaileddate.receipt_date=curdate()
)
as total
group by courier_company;

create view subview2 as
select courier_company,count(*) as '取件量' 
from 
(
select courier_company
from package,detaileddate 
where detaileddate.tracking_number=package.tracking_number 
and detaileddate.pickup_date=curdate()
)
as total
group by courier_company;

create view subview8 as
select package.courier_company,subview1.收件量
from package left join subview1 on package.courier_company=subview1.courier_company
group by courier_company;

create view subview9 as
select subview8.*,subview2.取件量
from subview8 left join subview2 on subview8.courier_company=subview2.courier_company;


########################################subview9为当日所有快递公司取件情况的最终视图#####################################################

create view subview3 as
select courier_company,count(*) as 收件量 
from package
group by courier_company;

create view subview4 as
select courier_company,count(*) as 取件量 
from package
where exist=false
group by courier_company;

create view subview5 as
select subview3.*,subview4.取件量
from subview3 left join subview4 on subview3.courier_company=subview4.courier_company;

create view subview6 as
select courier_company,count(*) as 未取件量 
from package
where exist=true
group by courier_company;

create view subview7 as
select subview5.*,subview6.未取件量
from subview5 left join subview6 on subview5.courier_company=subview6.courier_company;

###########################################subview7为历史所有快递公司取件情况的最终视图#########################################################