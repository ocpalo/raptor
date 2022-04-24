package com.bam.fuavserver.model.entity;

import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import javax.persistence.*;

@Entity
@Data
@AllArgsConstructor
@NoArgsConstructor
public class CrashInfo extends BaseEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @JsonIgnore
    private Long id;

    @OneToOne
    private Crash kilitlenmeBaslangicZamani;

    @OneToOne
    private Crash kilitlenmeBitisZamani;

    private Integer otonom_kilitlenme;

}
