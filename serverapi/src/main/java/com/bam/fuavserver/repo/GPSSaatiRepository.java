package com.bam.fuavserver.repo;

import com.bam.fuavserver.model.entity.GPSSaati;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface GPSSaatiRepository extends JpaRepository<GPSSaati, Long> {

}
