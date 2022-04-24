package com.bam.fuavserver.repo;

import com.bam.fuavserver.model.entity.CrashInfo;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface CrashInfoRepository extends JpaRepository<CrashInfo, Long> {
}
