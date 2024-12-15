all:
	cp ../../include/sinfourmis.h sinfourmis.h
	dune build
	opam install .

clean:
	dune clean
	rm -f sinfourmis.h
	opam uninstall sinfourmis
