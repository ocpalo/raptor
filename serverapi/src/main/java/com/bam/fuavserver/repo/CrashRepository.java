package com.bam.fuavserver.repo;

import com.bam.fuavserver.model.entity.Crash;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface CrashRepository extends JpaRepository<Crash, Long> {
}
