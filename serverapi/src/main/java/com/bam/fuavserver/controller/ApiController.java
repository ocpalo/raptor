package com.bam.fuavserver.controller;

import com.bam.fuavserver.model.dto.ServerClock;
import com.bam.fuavserver.model.entity.CrashInfo;
import com.bam.fuavserver.model.entity.Login;
import com.bam.fuavserver.model.entity.TelemetrySender;
import com.bam.fuavserver.model.response.TelemetryResponse;
import com.bam.fuavserver.service.ApiService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;


@RestController
@RequestMapping("api")
@RequiredArgsConstructor
public class ApiController {

    private final ApiService apiService;

    @GetMapping("sunucusaati")
    public ResponseEntity<ServerClock> getServerClock() {
        return ResponseEntity.ok(new ServerClock());
    }

    @PostMapping("telemetri_gonder")
    public ResponseEntity<TelemetryResponse> sendTelemetry(@RequestBody TelemetrySender telemetrySender){
        apiService.saveTelemetryResponse(telemetrySender);
        return new ResponseEntity(apiService.getTelemetryResponse(telemetrySender), HttpStatus.OK);
    }

    @PostMapping("kilitlenme_bilgisi")
    public ResponseEntity<CrashInfo> sendCrashInfo(@RequestBody CrashInfo crashInfo){
        return new ResponseEntity(apiService.saveCrashInfo(crashInfo), HttpStatus.OK);
    }

    @PostMapping("giris")
    public ResponseEntity Login(@RequestBody Login login){
        apiService.login(login);
        return new ResponseEntity(HttpStatus.OK);
    }

    @GetMapping("cikis")
    public ResponseEntity Exit(){
        apiService.exit();
        return new ResponseEntity(HttpStatus.OK);
    }

}
