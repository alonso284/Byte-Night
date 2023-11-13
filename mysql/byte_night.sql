DROP DATABASE IF EXISTS byte_night;
CREATE DATABASE IF NOT EXISTS byte_night;
USE byte_night;

SELECT 'CREATING DATABASE STRUCTURE' as 'INFO';

DROP TABLE IF EXISTS persons,
					 employees,
                     checks,
                     foods,
                     ingredients,
                     drinks,
					 ingredients_per_drink,
					 food_purchases,
					 drink_purchases,
					 -- dht_log,
					 -- volume_log;
					 log_temp,
					 log_volume,
					 log_uid,
					 log_pressure;

CREATE TABLE persons ( 
	id 				INT 			AUTO_INCREMENT PRIMARY KEY,
	curp 			CHAR(18) 		NOT NULL UNIQUE,
	uid 			BIGINT 			NOT NULL UNIQUE,
	first_name 		VARCHAR(50) 	NOT NULL,
	last_name 		VARCHAR(50) 	NOT NULL,
	sex 			ENUM('M', 'F')  NOT NULL,
	birth_date 		DATE 			NOT NULL,
	register_date 	DATE 			NOT NULL
	/* age is omitted due to redundancy */
);

CREATE TABLE employees (
    id 				INT 			AUTO_INCREMENT PRIMARY KEY,
    person_id 		INT 			NOT NULL,
    salary 			INT 			NOT NULL,
    FOREIGN KEY (person_id) REFERENCES persons(id)
);

CREATE TABLE checks ( 
	id 				INT 			AUTO_INCREMENT PRIMARY KEY,
	person_id 		INT 			NOT NULL,
	open_time 		DATETIME 		NOT NULL,
	close_time 		DATETIME,
	paid_for 		BOOLEAN 		NOT NULL DEFAULT FALSE,
	FOREIGN KEY (person_id) REFERENCES persons(id)
	/* due amount is omitted due to redundancy */
);

CREATE TABLE foods ( 
	id 				INT 			AUTO_INCREMENT PRIMARY KEY,
	name 			VARCHAR(50) 	NOT NULL UNIQUE,
	price 			DOUBLE 			NOT NULL,
	availability 	BOOLEAN 		NOT NULL DEFAULT FALSE
);

CREATE TABLE ingredients ( 
	id 				INT 			AUTO_INCREMENT PRIMARY KEY,
	name 			VARCHAR(50) 	NOT NULL UNIQUE,
	alcoholic 		BOOLEAN 		NOT NULL,
	availability 	BOOLEAN 		NOT NULL DEFAULT FALSE
);

CREATE TABLE drinks ( 
	id 				INT 			AUTO_INCREMENT PRIMARY KEY,
	name 			VARCHAR(50) 	NOT NULL UNIQUE,
	price 			DOUBLE 			NOT NULL,
	volume			INT				NOT NULL
);

CREATE TABLE ingredients_per_drink (
    drink_id INT NOT NULL,
    ingredient_id INT NOT NULL,
    percentage INT NOT NULL,
    PRIMARY KEY (drink_id, ingredient_id),
    FOREIGN KEY (drink_id) REFERENCES drinks(id),
    FOREIGN KEY (ingredient_id) REFERENCES ingredients(id)
);

CREATE TABLE food_purchases (
    id           	INT 			AUTO_INCREMENT PRIMARY KEY,
    time_stamp   	DATETIME 		NOT NULL,
    food_id      	INT 			NOT NULL,
    check_id     	INT 			NOT NULL,
    FOREIGN KEY (food_id) REFERENCES foods(id),
    FOREIGN KEY (check_id) REFERENCES checks(id)
);

CREATE TABLE drink_purchases (
    id           	INT 			AUTO_INCREMENT PRIMARY KEY,
    time_stamp   	DATETIME 		NOT NULL,
    drink_id     	INT 			NOT NULL,
    check_id     	INT 			NOT NULL,
    FOREIGN KEY (drink_id) REFERENCES drinks(id),
    FOREIGN KEY (check_id) REFERENCES checks(id)
);

-- CREATE TABLE dht_log ( 
-- 	time_stamp 		DATETIME 		PRIMARY KEY,
-- 	temperature 	INT 			NOT NULL,
-- 	humidity 		INT 			NOT NULL
-- );

-- CREATE TABLE volume_log ( 
-- 	time_stamp 		DATETIME 		PRIMARY KEY,
-- 	decibels 		INT 			NOT NULL
-- );

CREATE TABLE log_temp (
	log_id 			INT 			AUTO_INCREMENT PRIMARY KEY,
	log_date 		TIMESTAMP 		NOT NULL,
	temperature 	FLOAT 			NOT NULL,
	humidity 		FLOAT 			NOT NULL,
	active			BOOLEAN			NOT NULL
);

-- TABLA DE LECTOR DE VOLUMEN | ALE
CREATE TABLE log_volume (
	log_id 			INT 			AUTO_INCREMENT PRIMARY KEY,
	log_date 		TIMESTAMP 		NOT NULL,
	decibels 		INT 			NOT NULL,
	active			BOOLEAN			NOT NULL
);

-- TABALA DE RFID | DEBANHI
CREATE TABLE log_uid(
	log_id 			INT 			AUTO_INCREMENT PRIMARY KEY,
	log_date 		TIMESTAMP 		NOT NULL,
	uid 			BIGINT 			NOT NULL,
	allowed			BOOLEAN			NOT NULL
);

-- TABLA DE PRESION | DAVID
CREATE TABLE log_pressure(
	log_id 			INT 			AUTO_INCREMENT PRIMARY KEY,
	log_date 		TIMESTAMP 		NOT NULL,
    pressure 		INT 			NOT NULL,
	active			BOOLEAN			NOT NULL
);


source persons.dump
source employees.dump
source checks.dump
source foods.dump
source ingredients.dump
source drinks.dump
source ingredients_per_drink.dump
source food_purchases.dump
source drink_purchases.dump
source drink_purchases.dump
-- source dht_log.dump
-- source volume_log.dump
source log_temp.dump
source log_volume.dump
source log_uid.dump
source log_pressure.dump

