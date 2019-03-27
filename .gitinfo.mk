GIT_REPO_COMMIT	?= $(shell git describe --long --all --always --abbrev=8 | sed 's/.*-g\(.*\)/\1/')
GIT_REPO_BRANCH	?= $(shell git describe --long --all --always --abbrev=8 | cut -d- -f1 | cut -d/ -f2)
GIT_REPO_DESC	?= $(shell git describe --long --dirty --all --always --abbrev=8)
GIT_COMMIT_DATE ?= $(shell git show -s --format=%ci | cut -c1-10 | sed -e 's/-//g')
GIT_COMMIT_TIME ?= $(shell git show -s --format=%ci | cut -c12-19 | sed -e 's/://g')
ifeq ($(findstring dirty, $(GIT_REPO_DESC)), dirty)
GIT_REPO_DIRTY ?= 1
else
GIT_REPO_DIRTY ?= 0
endif
