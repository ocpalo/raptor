# Fighter_UAV_Server_API

Having Docker is enough for running application.

Clone this project and run command below in root directory:

    docker-compose up --build
    
In brief they will create two containers which names are fuavserver and h2db-fuav.

docker ps -- show executing containers

docker container prune -- remove nonexecuting containers

docker image prune -a -- remove images which have not any container
