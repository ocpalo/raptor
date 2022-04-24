package com.bam.fuavserver.model.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class LocationInfo {

    private Integer takim_numarasi;
    private Double iha_enlem;
    private Double iha_boylam;
    private Double iha_irtifa;
    private Double iha_dikilme;
    private Double iha_yonelme;
    private Double iha_yatis;

    private Integer zaman_farki;

}
