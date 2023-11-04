# API

## GET `/api/getLogs/:log_table`

Variables:
- `log_table` nombre de la tabla de la que quieren conseguir los logs

Tablas:
- `log_temp`
- `log_volume`
- `log_pressure`
- `log_uid`

Regresa los logs de la tabla indicada

## POST `/api/logTemp/:humidity/:temperature`

Variables:
- `humidity`
- `temperature`

## POST `/api/logVolume/:decibels`

Variables:
- `decibels`

## POST `/api/logUid/:uid`

Variables:
- `uid`

## POST `/api/logPressure/:pressure`

Variables:
- `pressure`

## GET `api/verifyUid/:uid`

Variables:
- `uid`

Regresa lista con personas `uid`s que coinciden

## POST `/api/purchaseDrink/:checkId/:drinkId`

Variables:
- `checkId`
- `drinkId`