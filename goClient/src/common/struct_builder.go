package common

import (
	"errors"
	"fmt"
	"log"
	"math"
	"strconv"
	"strings"
)

type StringSplitError struct {
	StatusCode int
	Err        error
}

func (r *StringSplitError) Error() string {
	return fmt.Sprintf("status %d: err %v", r.StatusCode, r.Err)
}

func BuildTelemetryRequest(i *TelemetryRequest, str string) error {
	strList := strings.Split(str, " ")
	if len(strList) != 19 {
		return &StringSplitError{
			StatusCode: -1,
			Err:        errors.New("parsed string array size is not correct"),
		}
	}
	i.TakimNumarasi, _ = strconv.Atoi(strList[0])
	i.IHAEnlem, _ = strconv.ParseFloat(strList[1], 64)
	i.IHABoylam, _ = strconv.ParseFloat(strList[2], 64)
	i.IHAIrtifa, _ = strconv.ParseFloat(strList[3], 64)
	i.IHADikilme, _ = strconv.ParseFloat(strList[4], 64)
	i.IHAYonelme, _ = strconv.ParseFloat(strList[5], 64)
	i.IHAYatis, _ = strconv.ParseFloat(strList[6], 64)
	i.IHAHiz, _ = strconv.ParseFloat(strList[7], 64)
	i.IHABatarya, _ = strconv.ParseFloat(strList[8], 64)
	i.IHAOtonom, _ = strconv.Atoi(strList[9])
	i.IHAKilitlenme, _ = strconv.Atoi(strList[10])
	i.HedefMerkezX, _ = strconv.ParseFloat(strList[11], 64)
	i.HedefMerkezY, _ = strconv.ParseFloat(strList[12], 64)
	i.HedefGenislik, _ = strconv.ParseFloat(strList[13], 64)
	i.HedefYukseklik, _ = strconv.ParseFloat(strList[14], 64)
	i.GPSSaati.Saat, _ = strconv.Atoi(strList[15])
	i.GPSSaati.Dakika, _ = strconv.Atoi(strList[16])
	i.GPSSaati.Saniye, _ = strconv.Atoi(strList[17])
	i.GPSSaati.Milisaniye, _ = strconv.Atoi(strList[18])
	return nil
}

func BuildLockInfo(i *LockInformation, str string) error {
	strList := strings.Split(str, ",")
	if len(strList) != 9 {
		return &StringSplitError{
			StatusCode: -1,
			Err:        errors.New("parsed string array size is not correct"),
		}
	}
	i.LockInit.Hour, _ = strconv.Atoi(strList[0])
	i.LockInit.Minute, _ = strconv.Atoi(strList[1])
	i.LockInit.Second, _ = strconv.Atoi(strList[2])
	i.LockInit.Millisecond, _ = strconv.Atoi(strList[3])
	i.LockEnd.Hour, _ = strconv.Atoi(strList[4])
	i.LockEnd.Minute, _ = strconv.Atoi(strList[5])
	i.LockEnd.Second, _ = strconv.Atoi(strList[6])
	i.LockEnd.Millisecond, _ = strconv.Atoi(strList[7])
	i.IsLockAutonomous, _ = strconv.Atoi(strList[8])
	return nil
}

// Send only 1 team KonumBilgileri instead of whole slice
func BuildTelemetryResponse(i *TelemetryResponse) string {
	var resp string
	resp += strconv.Itoa(i.SistemSaati.Hour) + ","
	resp += strconv.Itoa(i.SistemSaati.Minute) + ","
	resp += strconv.Itoa(i.SistemSaati.Second) + ","
	resp += strconv.Itoa(i.SistemSaati.Millisecond)

	index := findOptimumTarget(i, &TelemReq, LockedIndex)
	resp += "," + fmt.Sprintf("%d", i.KonumBilgileri[index].TakimNumarasi) + ","
	resp += fmt.Sprintf("%f", i.KonumBilgileri[index].IhaEnlem) + ","
	resp += fmt.Sprintf("%f", i.KonumBilgileri[index].IhaBoylam) + ","
	resp += fmt.Sprintf("%f", i.KonumBilgileri[index].IhaIrtifa) + ","
	resp += fmt.Sprintf("%f", i.KonumBilgileri[index].IhaDikilme) + ","
	resp += fmt.Sprintf("%f", i.KonumBilgileri[index].IhaYonelme) + ","
	resp += fmt.Sprintf("%f", i.KonumBilgileri[index].IhaYatis) + ","
	resp += fmt.Sprintf("%d", i.KonumBilgileri[index].ZamanFarki)

	return resp
}

func TestBuildTelemetryRequest() {
	var tStr string = "0 1.2 2.3 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17"
	log.Println(BuildTelemetryRequest(&TelemReq, tStr))
	log.Println(TelemReq)
}

func TestBuildLockInfo() {
	var tStr string = "0 1 2 3 4 5 6 7 8"
	log.Println(BuildLockInfo(&LockInfo, tStr))
	log.Println(LockInfo)
}

// TODO:: Modify this variable when locked info came
var LockedIndex = -1

func findOptimumTarget(i *TelemetryResponse, j *TelemetryRequest, previousLockedIndex int) (index int) {
	minDistance := math.MaxFloat64
	targetIndex := 0
	for index := 0; index < len(i.KonumBilgileri); index++ {
		if previousLockedIndex == i.KonumBilgileri[index].TakimNumarasi {
			continue
		}
		tmpDistance := distance(j.IHAEnlem, j.IHABoylam, float64(i.KonumBilgileri[index].IhaEnlem), float64(i.KonumBilgileri[index].IhaBoylam))
		if minDistance > tmpDistance {
			minDistance = tmpDistance
			targetIndex = index
		}
	}
	return targetIndex
}

const (
	earthRaidusM = 6372797.560856 // radius of the earth in meter.
)

func distance(src_lat, src_lon, dst_lat, dst_lon float64) (m float64) {
	lat1 := (src_lat * math.Pi / 180)
	lon1 := (src_lon * math.Pi / 180)
	lat2 := (dst_lat * math.Pi / 180)
	lon2 := (dst_lon * math.Pi / 180)

	diffLat := lat2 - lat1
	diffLon := lon2 - lon1

	a := math.Pow(math.Sin(diffLat/2), 2) + math.Cos(lat1)*math.Cos(lat2)*
		math.Pow(math.Sin(diffLon/2), 2)

	c := 2 * math.Atan2(math.Sqrt(a), math.Sqrt(1-a))
	m = c * earthRaidusM
	return m
}
