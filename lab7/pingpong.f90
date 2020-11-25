program ping_pong
	use mpi
	implicit none
	integer :: rank, nb_mpi_processes, ierror, tag, statu(MPI_STATUS_SIZE), n, pilka
	integer :: niter = 6

	! Ping Pong program

	tag = 7777

	call MPI_INIT( ierror )
	call MPI_COMM_SIZE( MPI_COMM_WORLD , nb_mpi_processes , ierror )
	call MPI_COMM_RANK( MPI_COMM_WORLD , rank , ierror )

	if(nb_mpi_processes /= 2) stop 'Ten program jest przeznaczony do uruchamiania tylko z 2 procesami'

	pilka = 0
	do n=1,niter

		if(rank==0) then
			call MPI_SEND ( pilka , 1 , MPI_INTEGER , 1 , tag , MPI_COMM_WORLD , ierror)
			call MPI_RECV ( pilka , 1 , MPI_INTEGER , 1 , tag , MPI_COMM_WORLD , statu , ierror )
			pilka = pilka + 2
		end if

		if(rank==1) then
			call MPI_RECV ( pilka , 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD , statu , ierror )
			pilka = pilka + 1
			call MPI_SEND ( pilka , 1 , MPI_INTEGER , 0 , tag , MPI_COMM_WORLD , ierror )
		end if

		print*, 'Proces',rank,'iter',n,'wartosc pileczki wynisi:',pilka

		call MPI_BARRIER(MPI_COMM_WORLD,ierror) ! A barrier. processes stop here, and can pass it only if ALL processes are here. Useful for debug, can impact performances

	end do

	call MPI_FINALIZE ( ierror ) ! Close MPI

end program ping_pong
