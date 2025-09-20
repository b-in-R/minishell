# Bugs identifiés et pistes de résolution

## 1. Quotes non fermées non détectées
- **Symptôme** : `minishell` acceptait des lignes comme `echo 'foo` ou `grep "bar` sans lever d'erreur, menant à un état incohérent.
- **Cause** : `has_unclosed_quotes()` (parser/syntax_checker.c) considérait la ligne valide si l’un des deux compteurs (quotes simples/doubles) était pair.
- **Solution** : exiger que les **deux** compteurs soient pairs, sinon retourner une erreur, comme dans Bash.

## 2. Assignations isolées mal détectées
- **Symptôme** : une ligne `FOO=bar ls` était traitée comme une simple assignation locale, et `ls` n’était jamais exécuté.
- **Cause** : `is_simple_assignment()` (expander/var.c) se déclenchait dès que la chaîne contenait un `=` sans vérifier la présence d’autres tokens.
- **Solution idéale** : laisser le lexer produire les tokens, puis vérifier que la liste ne contient que des `WORD` de type `IDENT=...`. Si oui, traiter comme assignation locale; sinon, poursuivre le parsing/exec. Cela garantit le comportement de Bash.

## 3. Heredoc toujours expandé
- **Symptôme** : `<<'EOF'` continuait à expanser `$VAR` alors que Bash ne le fait pas pour un délimiteur quoté.
- **Cause** : `t_cmd.expand_heredoc` n’est ni initialisé ni utilisé; l’info sur les quotes du délimiteur est perdue.
- **Solution** : stocker `tokens->next->quoted_type` dans la commande (`expand_heredoc`) et ne lancer `expand_variables()` que si le délimiteur n’était pas quoté.

## 4. Lignes composées uniquement d’espaces = erreur
- **Symptôme** : taper "   " renvoyait un statut d’erreur (2) au lieu d’être ignoré.
- **Cause** : `lexer()` retourne `NULL` pour une chaîne vide, `check_syntax_errors()` renvoie 1, et `process_input()` marque l’erreur.
- **Solution** : si `lexer()` renvoie `NULL`, considérer la ligne comme un no-op (status conservé) et ne passer au parser/exécution qu’en présence de tokens.

## 5. Blanks non gérés dans le lexer
- **Symptôme** : `echo\tfoo` ou d’autres espaces non ` ` ne sont pas reconnus comme séparateurs.
- **Cause** : `lexer()` ne teste que `' '` et ignore tabulation/espaces multiples.
- **Solution** : utiliser `ft_isspace()` (ou équivalent) pour sauter tous les blancs POSIX.

## 6. `$` sans identifiant : `$` supprimé
- **Symptôme** : commandes telles que `echo $$` ou `echo $+` perdaient le signe `$` ou donnaient des résultats incohérents.
- **Cause** : `exp_variable()` fabrique une clé vide si le caractère suivant n’est pas valide et renvoie `""`.
- **Solution** : conserver `$` comme littéral (en l’ajoutant à `result`) lorsque la clé est vide ou invalide, à l’instar de Bash.

## 7. `pwd` tue le shell sur erreur
- **Symptôme** : `PWD=$HOME && cd / && rm -rf . && pwd` peut déclencher `error_exit()` et quitter complètement `minishell`.
- **Cause** : `ft_pwd()` appelle `error_exit()`, qui fait `pool_cleanup_ctx()` et `exit(1)` dans le parent.
- **Solution** : faire remonter l’erreur en retournant un code non nul, sans appeler `error_exit()`; le shell peut alors continuer à tourner.
