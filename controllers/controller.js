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

    // async purchaseFood(req , res){
    //     console.log(req.params.checkId)
    //     console.log(req.params.foodId)
    //     if(req.params.checkId != null && req.params.foodId != null) {
    //         let checkId = req.params.checkId
    //         let foodId = req.params.foodId;
    //         var sql = `insert into food_purchases (time_stamp, food_id, check_id) values (now(), ${foodId}, ${checkId});`
    //         mysql.query(sql, (error,data,fields) => {
    //             if(error) {
    //                 res.status(500)
    //                 res.send(error.message)
    //             } else {
    //                 console.log(data)
                    // res.json({
                    //     status: 200,
                    //     message: "Log uploaded successfully",
                    //     affectedRows: data.affectedRows
                    // })
                // }
            // })
        // } else {
          // res.send('Por favor llena todos los datos!')
        // }
    // }
}

const controller = new MainController()
module.exports = controller;
