package com.bam.fuavserver.repo;

import com.bam.fuavserver.model.entity.Login;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface LoginRepository extends JpaRepository<Login, Long> {

    Login getLoginByKadi(String kadi);

}
