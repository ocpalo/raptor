package com.bam.fuavserver.model.converter;

import com.bam.fuavserver.model.dto.LocationInfo;
import com.bam.fuavserver.model.entity.TelemetrySender;
import org.springframework.stereotype.Component;
import org.springframework.util.CollectionUtils;

import java.util.Calendar;
import java.util.List;
import java.util.stream.Collectors;

@Component
public class LocationInfoConverter implements GenericConverter<List<TelemetrySender>, List<LocationInfo>> {

    @Override
    public List<LocationInfo> convert(List<TelemetrySender> telemetries) {
        if (CollectionUtils.isEmpty(telemetries)){
            return null;
        }

        return telemetries.stream().map(this::convertLocationInfo).collect(Collectors.toList());
    }

    private LocationInfo convertLocationInfo(TelemetrySender telemetries) {

        LocationInfo locationInfo = new LocationInfo();

        locationInfo.setIha_irtifa(telemetries.getIHA_irtifa());
        locationInfo.setIha_enlem(telemetries.getIHA_enlem());
        locationInfo.setIha_boylam(telemetries.getIHA_boylam());
        locationInfo.setTakim_numarasi(telemetries.getTakim_numarasi());
        locationInfo.setIha_dikilme(telemetries.getIHA_dikilme());
        locationInfo.setIha_yonelme(telemetries.getIHA_yonelme());
        locationInfo.setIha_yatis(telemetries.getIHA_yatis());
        locationInfo.setZaman_farki(getTimeDifference(telemetries));

        return locationInfo;
    }

    private Integer getTimeDifference(TelemetrySender telemetry) {
        Integer momentaryMs = Calendar.getInstance().get(Calendar.MILLISECOND);
        return telemetry.getGPSSaati() == null ? momentaryMs : Math.abs(momentaryMs - telemetry.getGPSSaati().getMilisaniye());
    }
}
