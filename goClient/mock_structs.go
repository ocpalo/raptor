package main

var loginInfo = TeamInfo{"go", "lang"}

var lockInfo = LockInfo{
	LockInit: struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	}{12, 20, 20, 80},
	LockEnd: struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	}{13, 20 ,20 ,80},
	IsLockAutonomous: 0,
}

var telemetry = TelemetryRequest{
	TakimNumarasi:  0,
	IHAEnlem:       1,
	IHABoylam:      2,
	IHAIrtifa:      3,
	IHADikilme:     4,
	IHAYonelme:     5,
	IHAYatis:       6,
	IHAHiz:         7,
	IHABatarya:     8,
	IHAOtonom:      1,
	IHAKilitlenme:  9,
	HedefMerkezX:   8,
	HedefMerkezY:   8,
	HedefGenislik:  8,
	HedefYukseklik: 8,
	GPSSaati: struct {
		Saat       int `json:"saat"`
		Dakika     int `json:"dakika"`
		Saniye     int `json:"saniye"`
		Milisaniye int `json:"milisaniye"`
	}{8, 13, 7, 87},
}

var telemetryResp = TelemetryResponse{}