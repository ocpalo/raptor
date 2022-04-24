package com.bam.fuavserver.service;

import com.bam.fuavserver.model.converter.GenericConverter;
import com.bam.fuavserver.model.dto.LocationInfo;
import com.bam.fuavserver.model.entity.CrashInfo;
import com.bam.fuavserver.model.entity.GPSSaati;
import com.bam.fuavserver.model.entity.Login;
import com.bam.fuavserver.model.entity.TelemetrySender;
import com.bam.fuavserver.model.response.TelemetryResponse;
import com.bam.fuavserver.repo.*;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Objects;

@Service
@RequiredArgsConstructor
public class ApiService {

    private final TelemetrySenderRepository telemetrySenderRepository;
    private final CrashInfoRepository crashInfoRepository;
    private final CrashRepository crashRepository;
    private final LoginRepository loginRepository;
    private final GPSSaatiRepository gpsSaatiRepository;

    private final GenericConverter<List<TelemetrySender>, List<LocationInfo>> locationInfoConverter;
    private final GenericConverter<List<LocationInfo>, TelemetryResponse> telemetryResponseConverter;

    public TelemetryResponse getTelemetryResponse(TelemetrySender telemetrySender){
        List<TelemetrySender> telemetries = telemetrySenderRepository.findAll();

        List<LocationInfo> locationInfoList = locationInfoConverter.convert(telemetries);
        locationInfoList.removeIf(locationInfo -> locationInfo.getTakim_numarasi().equals(telemetrySender.getTakim_numarasi()));

        return telemetryResponseConverter.convert(locationInfoList);
    }

    @Transactional
    public void saveTelemetryResponse(TelemetrySender telemetrySender){
        Long id = telemetrySenderRepository.getIdByTakimNo(telemetrySender.getTakim_numarasi());

        GPSSaati gpsSaati = telemetrySender.getGPSSaati();

        saveOrUpdateGpsSaati(telemetrySender, id, gpsSaati);

        telemetrySenderRepository.save(telemetrySender);

    }

    private void saveOrUpdateGpsSaati(TelemetrySender telemetrySender, Long id, GPSSaati gpsSaati) {
        if (Objects.nonNull(id)){
            gpsSaati.setId(id);
            gpsSaatiRepository.save(gpsSaati);
            telemetrySender.setId(id);
        }else {
            GPSSaati newGpsSaati = gpsSaatiRepository.save(gpsSaati);
            Long newId = newGpsSaati.getId();
            telemetrySender.setId(newId);
        }
    }

    @Transactional
    public CrashInfo saveCrashInfo(CrashInfo crashInfo){

        crashRepository.save(crashInfo.getKilitlenmeBaslangicZamani());
        crashRepository.save(crashInfo.getKilitlenmeBitisZamani());

        return crashInfoRepository.save(crashInfo);

    }

    public Boolean login(Login login){
        Login user = loginRepository.getLoginByKadi(login.getKadi());

        if(Objects.isNull(user)) {
            throw new RuntimeException("Invalid username");
        }

        if(user.getSifre().equals(login.getSifre())){
            user.setInOut(true);
            loginRepository.save(user);
            return true;
        }
        else {
            throw new RuntimeException("Invalid password");
        }
    }

    public void exit(){

        List<Login> loginList = loginRepository.findAll();

        loginList.forEach(login -> login.setInOut(false));
        loginList.forEach(loginRepository::save);
    }

}
