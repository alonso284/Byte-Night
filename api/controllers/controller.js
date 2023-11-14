const mysql = require('../database/db')

class MainController {

    async logTemp(req , res){
        console.log(req.params.humidity)
        console.log(req.params.temperature)
        if(req.params.humidity != null && req.params.temperature != null) {
            let humidity = req.params.humidity
            let temperature = req.params.temperature;
            let active;
            if(temperature > 20){
                active = 1;
            } else {  
                active = 0;
            }
            var sql = `insert into log_temp (log_date, temperature, humidity, active) values (now(), ${temperature}, ${humidity}, ${active});`
            mysql.query(sql, (error,data,fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        status: 200,
                        message: "Log uploaded successfully",
                        affectedRows: data.affectedRows
                    })
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }

    async logVolume(req , res){
        console.log(req.params.decibels)
        if(req.params.decibels != null) {
            let decibels = req.params.decibels;
            let active
            if(decibels > 100){
                active = 1;
            } else {
                active = 0;
            }
            var sql = `insert into log_volume (log_date, decibels, active) values (now(), ${decibels}, ${active});`
            mysql.query(sql, (error,data,fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        status: 200,
                        message: "Log uploaded successfully",
                        affectedRows: data.affectedRows
                    })
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }
    
    async logUid(req , res){
        console.log(req.params.uid)
        if(req.params.uid != null) {
            let uid = req.params.uid;
            var sql = `insert into log_uid (log_date, uid, allowed) values (now(), ${uid}, (select count(*) > 0 from persons where uid = ${uid}));`
            mysql.query(sql, (error,data,fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        status: 200,
                        message: "Log uploaded successfully",
                        affectedRows: data.affectedRows
                    })
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }

    async verifyUid(req,res){
        console.log("Verify UID")
        console.log(req.params.uid)
        if(req.params.uid!=null){
            let uid = req.params.uid;
            var sql = `SELECT * FROM persons where uid='${uid}'`
            mysql.query(sql, (error, data, fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        data
                    })
                }
            })
        }
    }

    async logPressure(req , res){
        console.log(req.params.pressure)
        if(req.params.pressure != null) {
            let pressure = req.params.pressure;
            let active;
            if(pressure < 13 && pressure > 1){
                active = 1;
            } else {  
                active = 0;
            }
            var sql = `insert into log_pressure (log_date, pressure, active) values (now(), ${pressure}, ${active});`
            mysql.query(sql, (error,data,fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        status: 200,
                        message: "Log uploaded successfully",
                        affectedRows: data.affectedRows
                    })
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }
    
    async getLogs(req,res){
        console.log("Get Logs")
        console.log(req.params.log_table)
        if(req.params.log_table!=null){
            let log_table = req.params.log_table;
            var sql = `SELECT * FROM ${log_table};`
            mysql.query(sql, (error, data, fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        data
                    })
                }
            })
        }
    }

    async getChecks(req,res){
        console.log("Get Checks")
        var sql = `SELECT c.id, c.person_id, p.first_name, p.last_name, c.open_time, c.close_time, paid_for, a.amount
        FROM checks AS c
        LEFT JOIN persons AS p ON p.id = c.person_id
        LEFT JOIN
        (
            SELECT COALESCE(fdp.check_id, sdp.check_id) AS check_id,
                   COALESCE(fdp.total, 0) + COALESCE(sdp.total, 0) AS amount
            FROM
              (SELECT fp.check_id, SUM(f.price) AS total
               FROM food_purchases AS fp
               LEFT JOIN foods AS f ON fp.food_id = f.id
               GROUP BY fp.check_id) fdp
            LEFT JOIN
              (SELECT dp.check_id, SUM(d.price) AS total
               FROM drink_purchases AS dp
               LEFT JOIN drinks AS d ON dp.drink_id = d.id
               GROUP BY dp.check_id) sdp
            ON sdp.check_id = fdp.check_id
        
            UNION
        
            SELECT COALESCE(fdp.check_id, sdp.check_id) AS check_id,
                   COALESCE(fdp.total, 0) + COALESCE(sdp.total, 0) AS amount
            FROM
              (SELECT fp.check_id, SUM(f.price) AS total
               FROM food_purchases AS fp
               LEFT JOIN foods AS f ON fp.food_id = f.id
               GROUP BY fp.check_id) fdp
            RIGHT JOIN
              (SELECT dp.check_id, SUM(d.price) AS total
               FROM drink_purchases AS dp
               LEFT JOIN drinks AS d ON dp.drink_id = d.id
               GROUP BY dp.check_id) sdp
            ON sdp.check_id = fdp.check_id
        ) AS a
        ON c.id = a.check_id
        ORDER BY c.paid_for, c.open_time ASC;`;
        mysql.query(sql, (error, data, fields) => {
            if(error) {
                res.status(500)
                res.send(error.message)
            } else {
                console.log(data)
                res.json({
                    data
                })
            }
        })
        
    }

    async purchase(req , res){
        console.log(req.params.type)
        console.log(req.params.personId)
        console.log(req.params.itemId)
        if(req.params.type != null && req.params.personId != null && req.params.itemId != null) {
            let type = req.params.type;
            let personId = req.params.personId;
            let itemId = req.params.itemId;
			var sql = `select id from checks where person_id=${personId} and paid_for=FALSE`;
            mysql.query(sql, (error, data, fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    // res.json({
                    //     data
                    // })
					if(data.length != 0){
						var checkId = data[0]["id"]
						var sql = `insert into ${type}_purchases (time_stamp, ${type}_id, check_id) values (now(), ${itemId}, ${checkId});`
						mysql.query(sql, (error,data,fields) => {
							if(error) {
								res.status(500)
								res.send(error.message)
							} else {
								console.log(data)
								res.json({
									status: 200,
									message: "Purchase successfully processed",
									affectedRows: data.affectedRows
								})
							}
						})
					} else {
          				res.send('Persona no tiene una cheque abierto')
					}
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }

    async closeTab(req , res){
        console.log(req.params.checkId)
        if(req.params.checkId != null) {
            let checkId = req.params.checkId;
            var sql = `update checks set close_time=now(), paid_for=TRUE where id=${checkId};`
            mysql.query(sql, (error,data,fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)
                    res.json({
                        status: 200,
                        message: "Log uploaded successfully",
                        affectedRows: data.affectedRows
                    })
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }

    async openTab(req , res){
        console.log(req.params.uid)
        if(req.params.uid != null) {
            let uid = req.params.uid;
			var sql = `select p.id, count(c.id) as tabs from persons as p left join checks as c on c.person_id = p.id and paid_for = 0 where p.uid = ${uid} group by p.id;`;
            mysql.query(sql, (error, data, fields) => {
                if(error) {
                    res.status(500)
                    res.send(error.message)
                } else {
                    console.log(data)

                    if (data.length === 0) {
                        res.send('uid no existe en la base de datos');
                        
                    } else {
                        if(data[0].tabs == 0){
                            var sql = `insert into checks (person_id, open_time) values (${data[0].id}, now());`
                            mysql.query(sql, (error,data,fields) => {
                                if(error) {
                                    res.status(500)
                                    res.send(error.message)
                                } else {
                                    console.log(data)
                                    res.json({
                                        status: 200,
                                        message: "Tab opened successfully",
                                        affectedRows: data.affectedRows
                                    })
                                }
                            })
                        } else {
                            res.send('Persona ya tiene una cheque abierto')
                        }
                    }
                }
            })
        } else {
          res.send('Por favor llena todos los datos!')
        }
    }
    
}

const controller = new MainController()
module.exports = controller;
