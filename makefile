export REP=$(PWD)
export REP_DESTINATION=$(REP)/Bin
export CPP=g++
export CXXFLAGS=-W -Wall -ansi -pedantic $(DEBUG)
export LIBDIR=-L"$(REP_DESTINATION)"

export CGI_DIR=$(REP)/Cgi
export COMMUN_DIR=$(REP)/Commun
export SITE_DIR=$(REP)/Site
export ADMINISTRATION_DIR=$(REP)/Administration
export INSCRIPTION_DIR=$(REP)/Inscription
export COMP_VISUEL_DIR=$(REP)/ComposantVisuel

export CGI_LIB=cgi
export COMMUN_LIB=commun

export EXEC_SITE=index.cgi
export EXEC_ADMINISTRATION=admin.cgi
export EXEC_INSCRIPTION=inscription.cgi
export EXEC_COMP_VISUEL=composant_visuel.cgi

all: $(EXEC_SITE) $(EXEC_ADMINISTRATION) $(EXEC_INSCRIPTION) $(EXEC_COMP_VISUEL)

$(EXEC_SITE): $(COMMUN_LIB)
	@(cd $(SITE_DIR) && $(MAKE) ; \
	cp $(SITE_DIR)/$(EXEC_SITE) $(REP_DESTINATION))

$(EXEC_ADMINISTRATION): $(COMMUN_LIB)
	@(cd $(ADMINISTRATION_DIR) && $(MAKE) ; \
	cp $(ADMINISTRATION_DIR)/$(EXEC_ADMINISTRATION) $(REP_DESTINATION))

$(EXEC_INSCRIPTION) : $(COMMUN_LIB)
	@(cd $(INSCRIPTION_DIR) && $(MAKE) ; \
	cp $(INSCRIPTION_DIR)/$(EXEC_INSCRIPTION) $(REP_DESTINATION))

$(EXEC_COMP_VISUEL) : $(CGI_LIB)
	@(cd $(COMP_VISUEL_DIR) && $(MAKE) ; \
	cp $(COMP_VISUEL_DIR)/$(EXEC_COMP_VISUEL) $(REP_DESTINATION))


$(CGI_LIB):
	@(cd $(CGI_DIR) && $(MAKE) ; \
	cp $(CGI_DIR)/lib$(CGI_LIB).so* $(REP_DESTINATION))

$(COMMUN_LIB): $(CGI_LIB)
	@(cd $(COMMUN_DIR) && $(MAKE) ; \
	cp $(COMMUN_DIR)/lib$(COMMUN_LIB).so* $(REP_DESTINATION))

.PHONY: clean mrproper debug

debug:
	@( $(MAKE) $(MAKEFILE) DEBUG="-g -O0 -D DEBUG" )

clean:
	@(rm -rf $(REP_DESTINATION)/* && \
	cd $(CGI_DIR) && $(MAKE) $@ && \
	cd $(COMMUN_DIR) && $(MAKE) $@ && \
	cd $(SITE_DIR) && $(MAKE) $@ && \
	cd $(ADMINISTRATION_DIR) && $(MAKE) $@ && \
	cd $(INSCRIPTION_DIR) && $(MAKE) $@ && \
	cd $(COMP_VISUEL_DIR) && $(MAKE) $@)

mrproper: clean
	@(rm -rf $(REP_DESTINATION)/* && \
	cd $(CGI_DIR) && $(MAKE) $@ && \
	cd $(COMMUN_DIR) && $(MAKE) $@ && \
	cd $(SITE_DIR) && $(MAKE) $@ && \
	cd $(ADMINISTRATION_DIR) && $(MAKE) $@ && \
	cd $(INSCRIPTION_DIR) && $(MAKE) $@ && \
	cd $(COMP_VISUEL_DIR) && $(MAKE) $@)

