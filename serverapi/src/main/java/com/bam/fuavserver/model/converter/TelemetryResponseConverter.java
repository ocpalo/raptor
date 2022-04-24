package com.bam.fuavserver.model.converter;

import com.bam.fuavserver.model.dto.LocationInfo;
import com.bam.fuavserver.model.dto.ServerClock;
import com.bam.fuavserver.model.response.TelemetryResponse;
import org.springframework.stereotype.Component;
import org.springframework.util.CollectionUtils;

import java.util.List;

@Component
public class TelemetryResponseConverter implements GenericConverter<List<LocationInfo>, TelemetryResponse> {


    @Override
    public TelemetryResponse convert(List<LocationInfo> locationInfoList) {

        if(CollectionUtils.isEmpty(locationInfoList)){
            return null;
        }

        TelemetryResponse telemetryResponse = new TelemetryResponse();
        telemetryResponse.setKonumBilgileri(locationInfoList);
        telemetryResponse.setSistemSaati(new ServerClock());

        return telemetryResponse;
    }
}
