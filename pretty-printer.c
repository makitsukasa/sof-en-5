#include "pretty-printer.h"

#if 0
#define SELEMOP_SERIAL, 1, {			1
#define SELEMOP_PARALLEL, 1, {		2
#define SELEMOP_ZERO_OR_MORE, 1, {	3
#define SELEMOP_ZERO_OR_ONE, 1, {		4
#define SELEMOP_SINGLE_TOKEN, 1, {	5
#endif

SyntaxElem syntaxElems[NUMOFSYNTAX];

int parse(int syntaxElem) {

	return 0;
}

int parse_init(void) {
	SyntaxElem sElem_TNAME				= {SELEMOP_SINGLE_TOKEN,	0, {}};
	SyntaxElem sElem_SPROGRAM			= {SELEMOP_SERIAL,			5, {TPROGRAM, TNAME, TSEMI, SBLOCK, TDOT}};
	SyntaxElem sElem_SBLOCK_1			= {SELEMOP_ZERO_OR_MORE,	1, {SBLOCK_1_1}};
	SyntaxElem sElem_SBLOCK_1_1			= {SELEMOP_PARALLEL,	    1, {SVARDEC , SSUBPROGDEC  }};
	SyntaxElem sElem_SVARDEC			= {SELEMOP_SERIAL,	        1, {TVAR , SVARNAMES, TCOLON, STYPE, TSEMI, SVARDEC_6              }};
	SyntaxElem sElem_SVARDEC_6			= {SELEMOP_ZERO_OR_MORE,	1, {SVARDEC_6_1                                              }};
	SyntaxElem sElem_SVARDEC_6_1		= {SELEMOP_SERIAL,	        4, {SVARNAMES, TCOLON, STYPE, TSEMI                             }};
	SyntaxElem sElem_SVARNAMES			= {SELEMOP_SERIAL,	        1, {SVARNAME , SVARNAMES_1                                     }};
	SyntaxElem sElem_SVARNAMES_1		= {SELEMOP_ZERO_OR_MORE,	1, {SVARNAMES_1_1
	SyntaxElem sElem_SVARNAMES_1_1		= {SELEMOP_SERIAL,	        2, {TCOMMA , SVARNAME                                          }};
	SyntaxElem sElem_SVARNAME			= {SELEMOP_SERIAL,	        1, {TNAME                                                    }};
	SyntaxElem sElem_STYPE				= {SELEMOP_PARALLEL,	    2, {SSTDTYPE, SARRTYPE                                        }};
	SyntaxElem sElem_SSTDTYPE			= {SELEMOP_PARALLEL,	    3, {TINTEGER, TBOOLEAN , TCHAR                                  }};
	SyntaxElem sElem_SARRTYPE			= {SELEMOP_SERIAL,	        6, {TARRAY , TLSQPAREN , TNUMBER , TRSQPAREN , TOF , SSTDTYPE          }};
	SyntaxElem sElem_SSUBPROGDEC		= {SELEMOP_SERIAL,	        7, {TPROCEDURE , SPROCEDURENAME , SSUBPROGDEC_3 , TSEMI , SSUBPROGDEC_5 , SCOMPSTAT , TSEMI                  }};
	SyntaxElem sElem_SSUBPROGDEC_3		= {SELEMOP_ZERO_OR_ONE,	    1, {SFORMPARAM                                             }};
	SyntaxElem sElem_SSUBPROGDEC_5		= {SELEMOP_ZERO_OR_ONE,	    1, {SVARDEC                                                  }};
	SyntaxElem sElem_SPROCEDURENAME		= {SELEMOP_SERIAL,	        1, {TNAME                                                    }};
	SyntaxElem sElem_SFORMPARAM			= {SELEMOP_SERIAL,	        1, {TLPAREN , SVARNAMES , TCOLON , STYPE , SFORMPARAM_5 , TRPAREN      }};
	SyntaxElem sElem_SFORMPARAM_5		= {SELEMOP_ZERO_OR_MORE,	1, {SFORMPARAM_5_1                                           }};
	SyntaxElem sElem_SFORMPARAM_5_1		= {SELEMOP_SERIAL,	        1, {TSEMI , SVARNAMES , TCOLON , STYPE                             }};
	SyntaxElem sElem_SCOMPSTAT			= {SELEMOP_SERIAL,	        1, {TBEGIN , SSTAT , SCOMPSTAT_3 , TEND                            }};
	SyntaxElem sElem_SCOMPSTAT_3		= {SELEMOP_ZERO_OR_MORE,	1, {SCOMPSTAT_3_1                                            }};
	SyntaxElem sElem_SCOMPSTAT_3_1		= {SELEMOP_SERIAL,	        1, {TSEMI , SSTAT                                              }};
	SyntaxElem sElem_SSTAT				= {SELEMOP_PARALLEL,	    1, {SASSIGNSTAT , SCONDSTAT , SITERSTAT , SEXITSTAT , SCALLSTAT , SRETSTAT , SINSTAT , SOUTSTAT , SCOMPSTAT , SEMPTYSTAT}};
	SyntaxElem sElem_SCONDSTAT			= {SELEMOP_SERIAL,	        1, {TIF , SEXPR , TTHEN , SSTAT , SCONDSTAT_5                        }};
	SyntaxElem sElem_SCONDSTAT_5		= {SELEMOP_ZERO_OR_ONE,	    1, {SCONDSTAT_5_1                                            }};
	SyntaxElem sElem_SCONDSTAT_5_1		= {SELEMOP_SERIAL,	        1, {TELSE , SSTAT }};
	SyntaxElem sElem_SITERSTAT			= {SELEMOP_SERIAL,	        1, {TWHILE , SSTAT , TDO , SSTAT , SITERSTAT_1 , SSTAT                 }};
	SyntaxElem sElem_SITERSTAT_1		= {SELEMOP_ZERO_OR_ONE,	    1, {TBREAK                                                   }};
	SyntaxElem sElem_SCALLSTAT			= {SELEMOP_SERIAL,	        1, {TCALL , SPROCEDURENAME , SCALLSTAT_3                         }};
	SyntaxElem sElem_SCALLSTAT_3		= {SELEMOP_ZERO_OR_ONE,	    1, {SCALLSTAT_3_1                                            }};
	SyntaxElem sElem_SCALLSTAT_3_1		= {SELEMOP_SERIAL,	        1, {TLPAREN , SEXPRS , TRPAREN                                   }};
	SyntaxElem sElem_SEXPRS				= {SELEMOP_SERIAL,	        1, {SEXPR , SEXPRS_2                                           }};
	SyntaxElem sElem_SEXPRS_2			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPRS_2_1                                               }};
	SyntaxElem sElem_SEXPRS_2_1			= {SELEMOP_SERIAL,	        1, {TCOMMA , SEXPR                                             }};
	SyntaxElem sElem_SRETSTAT			= {SELEMOP_SERIAL,	        1, {TRETURN                                                  }};
	SyntaxElem sElem_SASSIGNSTAT		= {SELEMOP_SERIAL,	        1, {SLEFTPART , TASSIGN , SEXPR                                  }};
	SyntaxElem sElem_SLEFTPART			= {SELEMOP_SERIAL,	        1, {SVAR                                                     }};
	SyntaxElem sElem_SVAR				= {SELEMOP_SERIAL,	        1, {SVARNAME , SVAR_2                                          }};
	SyntaxElem sElem_SVAR_2				= {SELEMOP_ZERO_OR_ONE,	    1, {SVAR_2_1                                                 }};
	SyntaxElem sElem_SVAR_2_1			= {SELEMOP_SERIAL,	        1, {TLSQPAREN , SEXPR , TRSQPAREN                                }};
	SyntaxElem sElem_SEXPR				= {SELEMOP_SERIAL,	        1, {SSIMPLEEXPR , SEXPR_2                                      }};
	SyntaxElem sElem_SEXPR_2			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPR_2_1                                                }};
	SyntaxElem sElem_SEXPR_2_1			= {SELEMOP_SERIAL,	        1, {SRELATOP , SSIMPLEEXPR                                     }};
	SyntaxElem sElem_SSIMPLEEXPR		= {SELEMOP_SERIAL,	        1, {SSIMPLEEXPR_1 , STERM , SSIMPLEEXPR_3                        }};
	SyntaxElem sElem_SSIMPLEEXPR_1		= {SELEMOP_ZERO_OR_ONE,	    1, {SSIMPLEEXPR_1_1                                          }};
	SyntaxElem sElem_SSIMPLEEXPR_1_1	= {SELEMOP_PARALLEL,	    1, {TPLUS , TMINUS                                             }};
	SyntaxElem sElem_SSIMPLEEXPR_3		= {SELEMOP_ZERO_OR_MORE,	1, {SSIMPLEEXPR_3_1                                          }};
	SyntaxElem sElem_SSIMPLEEXPR_3_1	= {SELEMOP_SERIAL,	        1, {SADDOP , STERM                                             }};
	SyntaxElem sElem_STERM				= {SELEMOP_SERIAL,	        1, {SFACTOR , STERM_2                                          }};
	SyntaxElem sElem_STERM_2			= {SELEMOP_ZERO_OR_MORE,	1, {STERM_2_1                                                }};
	SyntaxElem sElem_STERM_2_1			= {SELEMOP_SERIAL,	        1, {SMULOP , SFACTOR                                           }};
	SyntaxElem sElem_SFACTOR			= {SELEMOP_PARALLEL,	    1, {SVAR , SCONST , SFACTOR_3 , SFACTOR_4 , SFACTOR_5                }};
	SyntaxElem sElem_SFACTOR_3			= {SELEMOP_SERIAL,	        1, {TLPAREN , SEXPR , TRPAREN                                    }};
	SyntaxElem sElem_SFACTOR_4			= {SELEMOP_SERIAL,	        1, {TNOT , SFACTOR                                             }};
	SyntaxElem sElem_SFACTOR_5			= {SELEMOP_SERIAL,	        1, {SSTDTYPE , TLPAREN , SEXPR , TRPAREN                           }};
	SyntaxElem sElem_SCONST				= {SELEMOP_PARALLEL,	    1, {TNUMBER , TFALSE , TTRUE , TSTRING                             }};
	SyntaxElem sElem_SMULOP				= {SELEMOP_PARALLEL,	    1, {TSTAR , TDIV , TAND                                          }};
	SyntaxElem sElem_SADDOP				= {SELEMOP_PARALLEL,	    1, {TPLUS , TMINUS , TOR                                         }};
	SyntaxElem sElem_SRELATOP			= {SELEMOP_PARALLEL,	    1, {TEQUAL , TNOTEQ , TLE , TLEEQ , TGR , TGREQ                        }};
	SyntaxElem sElem_SINSTAT			= {SELEMOP_SERIAL,	        1, {SINSTAT_1 , SINSTAT_2                                      }};
	SyntaxElem sElem_SINSTAT_1			= {SELEMOP_PARALLEL,	    1, {TREAD , TREADLN                                            }};
	SyntaxElem sElem_SINSTAT_2			= {SELEMOP_ZERO_OR_ONE,	    1, {SINSTAT_2_1                                              }};
	SyntaxElem sElem_SINSTAT_2_1		= {SELEMOP_SERIAL,	        1, {TLPAREN , SVAR , SINSTAT_2_1_3 , TRPAREN                       }};
	SyntaxElem sElem_SINSTAT_2_1_3		= {SELEMOP_ZERO_OR_MORE,	1, {SINSTAT_2_1_3_1                                          }};
	SyntaxElem sElem_SINSTAT_2_1_3_1	= {SELEMOP_SERIAL,	        1, {TCOMMA , SVAR                                              }};
	SyntaxElem sElem_SOUTSTAT			= {SELEMOP_SERIAL,	        1, {SOUTSTAT_1 , SOUTSTAT_2                                    }};
	SyntaxElem sElem_SOUTSTAT_1			= {SELEMOP_PARALLEL,	    1, {TWRITE , TWRITELN                                          }};
	SyntaxElem sElem_SOUTSTAT_2			= {SELEMOP_ZERO_OR_ONE,	    1, {SOUTSTAT_2_1                                             }};
	SyntaxElem sElem_SOUTSTAT_2_1		= {SELEMOP_SERIAL,	        1, {TLPAREN , SOUTFORM , SOUTSTAT_2_1_3                          }};
	SyntaxElem sElem_SOUTSTAT_2_1_3		= {SELEMOP_ZERO_OR_MORE,	1, {SOUTSTAT_2_1_3_1                                         }};
	SyntaxElem sElem_SOUTSTAT_2_1_3_1	= {SELEMOP_SERIAL,	        1, {TCOMMA , SOUTFORM                                          }};
	SyntaxElem sElem_SOUTFORM			= {SELEMOP_PARALLEL,	    1, {SOUTFORM_1 , TSTRING                                       }};
	SyntaxElem sElem_SOUTFORM_1			= {SELEMOP_SERIAL,	        1, {SEXPR , SOUTFORM_1_2                                       }};
	SyntaxElem sElem_SOUTFORM_1_2		= {SELEMOP_ZERO_OR_ONE,	    1, {SOUTFORM_1_2_1                                           }};
	SyntaxElem sElem_SOUTFORM_1_2_1		= {SELEMOP_SERIAL,			1, {TCOLON , TNUMBER                                           }};
	SyntaxElem sElem_SEMPTYSTAT			= {SELEMOP_SINGLE_TOKEN,	0, {}};

	syntaxElems[SPROGRAM] = sElem_SPROGRAM;

	return 0;
}

int main(void) {
	parse_init();
	/*printf("%d\n", parse(SPROGRAM));*/
	return 0;
}
