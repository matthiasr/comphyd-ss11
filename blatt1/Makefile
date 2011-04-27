GNUPLOT?= gnuplot
PYTHON?= python

PDFS= aufgabe1-0.0001.pdf aufgabe1-0.001.pdf aufgabe1-0.01.pdf aufgabe1-0.1.pdf aufgabe1-errors.pdf aufgabe2-0.0001.pdf aufgabe2-0.001.pdf aufgabe2-0.01.pdf aufgabe2-0.1.pdf aufgabe2-errors.pdf

all: $(PDFS)

$(PDFS): blatt1.py
	$(PYTHON) $< | $(GNUPLOT)

.PHONY: clean

clean:
	rm -f $(PDFS)
	rm -f *.pyc
