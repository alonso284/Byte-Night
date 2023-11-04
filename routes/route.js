const express = require('express');
const controller = require('../controllers/controller');
const router = express.Router();

router.get('/api/getLogs/:log_table', controller.getLogs);
router.get('/api/verifyUid/:uid', controller.verifyUid);

router.post('/api/logTemp/:humidity/:temperature', controller.logTemp);
router.post('/api/logVolume/:decibels', controller.logVolume);
router.post('/api/logUid/:uid', controller.logUid);
router.post('/api/logPressure/:pressure', controller.logPressure);

module.exports = router;
