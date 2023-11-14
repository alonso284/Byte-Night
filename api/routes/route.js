const express = require('express');
const controller = require('../controllers/controller');
const router = express.Router();

router.get('/api/getLogs/:log_table', controller.getLogs);
router.get('/api/verifyUid/:uid', controller.verifyUid);

router.get('/api/getChecks', controller.getChecks);

router.post('/api/logTemp/:humidity/:temperature', controller.logTemp);
router.post('/api/logVolume/:decibels', controller.logVolume);
router.post('/api/logUid/:uid', controller.logUid);
router.post('/api/logPressure/:pressure', controller.logPressure);
router.post('/api/purchase/:type/:personId/:itemId', controller.purchase);

router.post('/api/closeTab/:checkId', controller.closeTab);
router.post('/api/openTab/:uid', controller.openTab);

module.exports = router;
