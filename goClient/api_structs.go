package main

type ServerTime struct {
	Hour   		int `json:"saat"`
	Minute 		int `json:"dakika"`
	Second      int `json:"saniye"`
	Millisecond int `json:"milisaniye"`
}

type TeamInfo struct {
	Username string `json:"kadi"`
	Passwd   string `json:"sifre"`
}

type LockInfo struct {
	LockInit struct {
		Hour       int `json:"saat"`
		Minute     int `json:"dakika"`
		Second     int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	} `json:"kilitlenmeBaslangicZamani"`
	LockEnd struct {
		Hour       int `json:"saat"`
		Minute     int `json:"dakika"`
		Second     int `json:"saniye"`
		Millisecond int `json:"milisaniye"`
	} `json:"kilitlenmeBitisZamani"`
	IsLockAutonomous int `json:"otonom_kilitlenme"`
}

type TelemetryResponse struct {
	SistemSaati struct {
		Hour      	 int `json:"saat"`
		Minute    	 int `json:"dakika"`
		Second    	 int `json:"saniye"`
		Millisecond	 int `json:"milisaniye"`
	} `json:"sistemSaati"`
	KonumBilgileri []struct {
		TakimNumarasi int `json:"takim_numarasi"`
		IhaEnlem      int `json:"iha_enlem"`
		IhaBoylam     int `json:"iha_boylam"`
		IhaIrtifa     int `json:"iha_irtifa"`
		IhaDikilme    int `json:"iha_dikilme"`
		IhaYonelme    int `json:"iha_yonelme"`
		IhaYatis      int `json:"iha_yatis"`
		ZamanFarki    int `json:"zaman_farki"`
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
