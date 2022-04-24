package com.bam.fuavserver.model.entity;

import com.fasterxml.jackson.annotation.JsonAlias;
import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import javax.persistence.*;

@Entity
@Data
@AllArgsConstructor
@NoArgsConstructor
public class TelemetrySender extends BaseEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonIgnore
    private Long id;

    private Integer takim_numarasi;

    @JsonAlias("IHA_enlem")
    private Double IHA_enlem;

    @JsonAlias("IHA_boylam")
    private Double IHA_boylam;

    @JsonAlias("IHA_irtifa")
    private Double IHA_irtifa;

    @JsonAlias("IHA_dikilme")
    private Double IHA_dikilme;

    @JsonAlias("IHA_yonelme")
    private Double IHA_yonelme;

    @JsonAlias("IHA_yatis")
    private Double IHA_yatis;

    @JsonAlias("IHA_hiz")
    private Double IHA_hiz;

    @JsonAlias("IHA_batarya")
    private Double IHA_batarya;

    @JsonAlias("IHA_otonom")
    private Integer IHA_otonom;

    @JsonAlias("IHA_kilitlenme")
    private Integer IHA_kilitlenme;

    @JsonAlias("Hedef_merkez_X")
    private Double Hedef_merkez_X;

    @JsonAlias("Hedef_merkez_Y")
    private Double Hedef_merkez_Y;

    @JsonAlias("Hedef_genislik")
    private Double Hedef_genislik;

    @JsonAlias("Hedef_yukseklik")
    private Double Hedef_yukseklik;

    @OneToOne
    @JsonAlias("GPSSaati")
    private GPSSaati GPSSaati;
}
