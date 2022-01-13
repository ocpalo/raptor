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
		IhaEnlem      float32 `json:"iha_enlem"`
		IhaBoylam     float32 `json:"iha_boylam"`
		IhaIrtifa     float32 `json:"iha_irtifa"`
		IhaDikilme    float32 `json:"iha_dikilme"`
		IhaYonelme    float32 `json:"iha_yonelme"`
		IhaYatis      float32 `json:"iha_yatis"`
		ZamanFarki    float32 `json:"zaman_farki"`
	} `json:"konumBilgileri"`
}

type TelemetryRequest struct {
	TakimNumarasi  int     `json:"takim_numarasi"`
	IHAEnlem       float64 `json:"IHA_enlem"`
	IHABoylam      float64 `json:"IHA_boylam"`
	IHAIrtifa      float64 `json:"IHA_irtifa"`
	IHADikilme     int     `json:"IHA_dikilme"`
	IHAYonelme     int     `json:"IHA_yonelme"`
	IHAYatis       int     `json:"IHA_yatis"`
	IHAHiz         int     `json:"IHA_hiz"`
	IHABatarya     int     `json:"IHA_batarya"`
	IHAOtonom      int     `json:"IHA_otonom"`
	IHAKilitlenme  int     `json:"IHA_kilitlenme"`
	HedefMerkezX   int     `json:"Hedef_merkez_X"`
	HedefMerkezY   int     `json:"Hedef_merkez_Y"`
	HedefGenislik  int     `json:"Hedef_genislik"`
	HedefYukseklik int     `json:"Hedef_yukseklik"`
	GPSSaati       struct {
		Saat       int `json:"saat"`
		Dakika     int `json:"dakika"`
		Saniye     int `json:"saniye"`
		Milisaniye int `json:"milisaniye"`
	} `json:"GPSSaati"`
}
