FROM openjdk:8
WORKDIR /app
ADD target/fuavserver-0.0.1-SNAPSHOT.jar fuavserver-0.0.1-SNAPSHOT.jar
EXPOSE 8080
ENTRYPOINT ["java", "-jar", "fuavserver-0.0.1-SNAPSHOT.jar"]