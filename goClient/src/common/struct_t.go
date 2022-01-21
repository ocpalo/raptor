package common

type LogOut struct{}

type ServerTime struct {
	Hour        int `json:"saat"`
	Minute      int `json:"dakika"`
	Second      int `json:"saniye"`
	Millisecond int `json:"milisaniye"`
}

type TeamInformation struct {
	Username string `json:"kadi"`
	Passwd   string `json:"sifre"`
}

type LockInformation struct {
	LockInit struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	} `json:"kilitlenmeBaslangicZamani"`
	LockEnd struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	} `json:"kilitlenmeBitisZamani"`
	IsLockAutonomous int `json:"otonom_kilitlenme"`
}

type TelemetryResponse struct {
	SistemSaati struct {
		Hour        int `json:"saat"`
		Minute      int `json:"dakika"`
		Second      int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	} `json:"sistemSaati"`
	KonumBilgileri []struct {
		TakimNumarasi int     `json:"takim_numarasi"`
		IhaEnlem      float64 `json:"iha_enlem"`
		IhaBoylam     float64 `json:"iha_boylam"`
		IhaIrtifa     float64 `json:"iha_irtifa"`
		IhaDikilme    float64 `json:"iha_dikilme"`
		IhaYonelme    float64 `json:"iha_yonelme"`
		IhaYatis      float64 `json:"iha_yatis"`
		ZamanFarki    int     `json:"zaman_farki"`
	} `json:"konumBilgileri"`
}

type TelemetryRequest struct {
	TakimNumarasi  int     `json:"takim_numarasi"`
	IHAEnlem       float64 `json:"IHA_enlem"`
	IHABoylam      float64 `json:"IHA_boylam"`
	IHAIrtifa      float64 `json:"IHA_irtifa"`
	IHADikilme     float64 `json:"IHA_dikilme"`
	IHAYonelme     float64 `json:"IHA_yonelme"`
	IHAYatis       float64 `json:"IHA_yatis"`
	IHAHiz         float64 `json:"IHA_hiz"`
	IHABatarya     float64 `json:"IHA_batarya"`
	IHAOtonom      int     `json:"IHA_otonom"`
	IHAKilitlenme  int     `json:"IHA_kilitlenme"`
	HedefMerkezX   float64 `json:"Hedef_merkez_X"`
	HedefMerkezY   float64 `json:"Hedef_merkez_Y"`
	HedefGenislik  float64 `json:"Hedef_genislik"`
	HedefYukseklik float64 `json:"Hedef_yukseklik"`
	GPSSaati       struct {
		Saat       int `json:"saat"`
		Dakika     int `json:"dakika"`
		Saniye     int `json:"saniye"`
		Milisaniye int `json:"milisaniye"`
	} `json:"GPSSaati"`
}
