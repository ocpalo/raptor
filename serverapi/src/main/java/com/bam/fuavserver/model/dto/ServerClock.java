package com.bam.fuavserver.model.dto;

import lombok.Data;

import java.util.Calendar;

@Data
public class ServerClock {

    private Integer saat;

    private Integer dakika;

    private Integer saniye;

    private Integer milisaniye;

    public ServerClock(){
        Calendar time = Calendar.getInstance();
        saat = time.get(Calendar.HOUR_OF_DAY);
        dakika = time.get(Calendar.MINUTE);
        saniye = time.get(Calendar.SECOND);
        milisaniye = time.get(Calendar.MILLISECOND);
    }

}

