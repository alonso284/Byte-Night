const mysql = require('../database/db')

class MainController {

    async logTemp(req , res){
        console.log(req.params.humidity)
        console.log(req.params.temperature)
        if(req.params.humidity != null && req.params.temperature != null) {
            let humidity = req.params.humidity
            let temperature = req.params.temperature;
            var sql = `insert into log_temp (log_date, temperature, humidity) values (now(), ${temperature}, ${humidity});`
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
            var sql = `insert into log_volume (log_date, decibels) values (now(), ${decibels});`
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
            var sql = `insert into log_uid (log_date, uid) values (now(), ${uid});`
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
            var sql = `insert into log_pressure (log_date, pressure) values (now(), ${pressure});`
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

    async purchaseDrink(req , res){
        console.log(req.params.checkId)
        console.log(req.params.drinkId)
        if(req.params.checkId != null && req.params.drinkId != null) {
            let checkId = req.params.checkId
            let drinkId = req.params.drinkId;
            var sql = `insert into drink_purchases (time_stamp, drink_id, check_id) values (now(), ${drinkId}, ${checkId});`
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
}

const controller = new MainController()
module.exports = controller;
