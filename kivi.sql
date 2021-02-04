create database kivi_db;

USE kivi_db;

CREATE TABLE Users (
    Id int NOT NULL AUTO_INCREMENT,
    Type int NOT NULL,
    FirstName varchar(100) NOT NULL,
    LastName varchar(100) NOT NULL,
    NationalCode varchar(10) NOT NULL,
    Username varchar(50) NOT NULL,
    Password varchar(50) NOT NULL,
    Email varchar(100) NOT NULL,
    CoronaCode int NOT NULL,
    CodeStamp int NOT NULL,
    PRIMARY KEY (ID)
);

CREATE TABLE NormalInfo (
    UserId int NOT NULL,
    BloodGroup varchar(3),
    Age int,
    Height int,
    Weight int,
    PRIMARY KEY (UserId)
);

CREATE TABLE BusInfo (
    UserId int NOT NULL,
    PlaceName varchar(100) NOT NULL,
    CityName varchar(100) NOT NULL,
    AreaId int NOT NULL,
    PRIMARY KEY (UserId)
);

CREATE TABLE Hospitals (
    Id int NOT NULL AUTO_INCREMENT,
    Name varchar(100) NOT NULL,
    AreaId int NOT NULL,
    PRIMARY KEY (Id)
);

CREATE TABLE AreaVisits (
    UserId int NOT NULL,
    AreaId int NOT NULL,
    Stamp int NOT NULL
);

CREATE TABLE UserVisits (
    UserId1 int NOT NULL,
    UserId2 int NOT NULL,
    Stamp int NOT NULL
);

CREATE TABLE CoronaTests (
    UserId int NOT NULL,
    CoronaCode int NOT NULL,
    Stamp int NOT NULL
);

CREATE TABLE Messages (
    FromId int NOT NULL,
    ToId int NOT NULL,
    Text varchar(1000) NOT NULL,
    Stamp int NOT NULL
);

INSERT INTO Users (Type, FirstName, LastName, NationalCode, Username, Password, Email, CoronaCode, CodeStamp)
VALUES (3, 'admin', 'admin', '0', 'admin', 'firns678', 'admin', 0, 0);
//firns678 = admin123
