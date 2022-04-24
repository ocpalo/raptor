package com.bam.fuavserver.repo;

import com.bam.fuavserver.model.entity.TelemetrySender;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

@Repository
public interface TelemetrySenderRepository extends JpaRepository<TelemetrySender, Long> {

    @Query(value = "SELECT t.id FROM TELEMETRY_SENDER t WHERE t.takim_numarasi=:number",nativeQuery = true)
    Long getIdByTakimNo(@Param("number") Integer takim_no);

}
