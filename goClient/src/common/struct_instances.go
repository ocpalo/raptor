package common

var LoginInfo = TeamInformation{"1", "1"}

var LockInfo = LockInformation{
	LockInit: struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	}{0, 0, 0, 0},
	LockEnd: struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	}{0, 0, 0, 0},
	IsLockAutonomous: 0,
}

var TelemReq = TelemetryRequest{
	TakimNumarasi:  0,
	IHAEnlem:       0,
	IHABoylam:      0,
	IHAIrtifa:      0,
	IHADikilme:     0,
	IHAYonelme:     0,
	IHAYatis:       0,
	IHAHiz:         0,
	IHABatarya:     0,
	IHAOtonom:      0,
	IHAKilitlenme:  0,
	HedefMerkezX:   0,
	HedefMerkezY:   0,
	HedefGenislik:  0,
	HedefYukseklik: 0,
	GPSSaati: struct {
		Saat       int `json:"saat"`
		Dakika     int `json:"dakika"`
		Saniye     int `json:"saniye"`
		Milisaniye int `json:"milisaniye"`
	}{0, 0, 0, 0},
}

var TelemResp = TelemetryResponse{}
