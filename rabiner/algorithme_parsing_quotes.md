# ALGORITHME DE PARSING DES QUOTES - by jimouleboos

## Vue d'ensemble

```
┌─────────────────────────────────────────────────────────────┐
│                    ALGORITHME PRINCIPAL                     │
│                                                             │
│  read_input() → tokenize() → state_machine() → result       │
│                                                             │
│  Principe: PREMIÈRE QUOTE GAGNE                             │
│  - Si ' rencontrée en premier → tout jusqu'à ' fermante     │
│  - Si " rencontrée en premier → tout jusqu'à " fermante     │
│  - Quotes imbriquées = caractères littéraux                 │
└─────────────────────────────────────────────────────────────┘
```

## CAS 1: echo "'s""s'" (déjà dans le schéma)

**Résumé rapide:**
- État: `e_default` → rencontre `'` → `e_in_squotes`
- Tout entre les `'` devient littéral (including `"`)
- Résultat: `"s""s`

## CAS 2: echo "bonjour" "salut"

### Position des caractères
```
Position: e c h o   " b o n j o u r "   " s a l u t "
          ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑
Step:     0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
```

### Cheminement algorithmique

#### Pos 0-4: "echo "
```
┌─────────────────────┐
│ État: e_default     │ → Accumule "echo"
│ Action: create_token│ → TOKEN 1: "echo" (e_word)
│ Espace: skip        │ → Avance à pos 5
└─────────────────────┘
```

#### Pos 5: `"` (première quote)
```
┌─────────────────────┐
│ État: e_default     │ → Rencontre " → handle_quote()
│ Action: Ouvre "     │ → create_new_token()
│ Nouveau:e_in_dquotes│ → PAS de stockage du "
└─────────────────────┘
```

#### Pos 6-12: `bonjour`
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Caractères normaux
│ Action: append_char │ → b,o,n,j,o,u,r
│ Token: "bonjour     │ → Accumulation en cours
└─────────────────────┘
```

#### Pos 13: `"` (quote fermante)
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Trouve " fermante!
│ Action: Ferme quote │ → return, état = e_default
│ Token final: bonjour│ → Type: e_dquote
└─────────────────────┘
```

#### Pos 14: ` ` (espace)
```
┌─────────────────────┐
│ État: e_default     │ → Skip espace
│ Action: next_char   │ → Avance à pos 15
│ Token: stocké       │ → TOKEN 2: "bonjour"
└─────────────────────┘
```

#### Pos 15: `"` (nouvelle quote)
```
┌─────────────────────┐
│ État: e_default     │ → Rencontre " → handle_quote()
│ Action: Ouvre "     │ → create_new_token()
│ Nouveau:e_in_dquotes│ → Nouveau cycle
└─────────────────────┘
```

#### Pos 16-21: `salut`
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Caractères normaux
│ Action: append_char │ → s,a,l,u,t
│ Token: "salut       │ → Accumulation
└─────────────────────┘
```

#### Pos 22: `"` (quote fermante)
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Trouve " fermante!
│ Action: Ferme quote │ → return, état = e_default
│ Token final: salut  │ → Type: e_dquote
└─────────────────────┘
```

**Résultat CAS 2:**
```
┌─────────────────────┐
│ TOKEN 1: "echo"     │ Type: e_word
│ TOKEN 2: bonjour    │ Type: e_dquote
│ TOKEN 3: salut      │ Type: e_dquote
└─────────────────────┘
```

## CAS 3: "bonjour""salut"

### Position des caractères
```
Position: " b o n j o u r " " s a l u t "
          ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑
Step:     0 1 2 3 4 5 6 7 8 9 10 11 12 13 
```

### Cheminement algorithmique

#### Pos 0: `"` (première quote)
```
┌─────────────────────┐
│ État: e_default     │ → Rencontre " → handle_quote()
│ Action: Ouvre "     │ → create_new_token()
│ Nouveau:e_in_dquotes│ → Premier contexte
└─────────────────────┘
```

#### Pos 1-7: `bonjour`
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Accumulation normale
│ Action: append_chars│ → b,o,n,j,o,u,r
│ Token: "bonjour     │
└─────────────────────┘
```

#### Pos 8: `"` (quote fermante)
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Trouve " fermante!
│ Action: Ferme quote │ → return, état = e_default
│ Token: bonjour      │ → PAS finalisé encore!
└─────────────────────┘
```

#### Pos 9: `"` (quote adjacente)
```
┌─────────────────────┐
│ État: e_default     │ → Rencontre " IMMÉDIATEMENT
│ Action: MÊME TOKEN  │ → handle_quote() CONTINUE
│ Nouveau:e_in_dquotes│ → MÊME token réutilisé!
└─────────────────────┘
```

#### Pos 10-15: `salut`
```
┌─────────────────────┐
│ État: e_in_dquotes  │ → Accumulation SUITE
│ Action: append_chars│ → s,a,l,u,t
│ Token: bonjoursalut │ → CONCATÉNATION!
└─────────────────────┘
```

#### Pos 15: `"` (quote fermante finale)
```
┌───────────────────────────┐
│ État: e_in_dquotes        │ → Trouve " fermante!
│ Action: Ferme quote       │ → return, état = e_default
│ Token final: bonjoursalut │ → Type: e_dquote
└───────────────────────────┘
```

**Résultat CAS 3:**
```
┌────────────────────────────┐
│ TOKEN UNIQUE: bonjoursalut │ Type: e_dquote
└────────────────────────────┘
```

## Règles algorithmiques

### 1. Gestion des transitions d'état
```
e_default → (quote) → e_in_quotes → (quote fermante) → e_default
                                                    ↓
                        Si quote IMMÉDIATE → CONTINUE même token
                        Si espace/autre → FINALISE token
```

### 2. Concaténation automatique
```
"texte1""texte2" → UN SEUL TOKEN: texte1texte2
"texte1" "texte2" → DEUX TOKENS: texte1, texte2
        ↑
Espace = séparateur
```

### 3. Priorité de parsing
```
PREMIÈRE QUOTE DÉTERMINE LE CONTEXTE:
'texte"autre"fin' → Tout littéral (y compris ")
"texte'autre'fin" → Tout expansible (y compris ')
```

### 4. États de l'automate
```
┌─────────────┐ quote ┌──────────────┐ quote ┌─────────────┐
│ e_default   │──────→│ e_in_quotes  │──────→│ e_default   │
│             │       │              │       │             │
└─────────────┘       └──────────────┘       └─────────────┘
      ↑                                             │
      └─────────────────────────────────────────────┘
                    Cycle peut se répéter
```

## Différences entre les trois cas

| Cas | Structure | Résultat | Raison |
|-----|-----------|----------|---------|
| `"'s""s'"` | Quotes imbriquées | `"s""s` | `'` prioritaire, `"` littéral |
| `"bonjour" "salut"` | Quotes séparées | `bonjour`, `salut` | Espace = séparateur |
| `"bonjour""salut"` | Quotes adjacentes | `bonjoursalut` | Concaténation auto |

## pseudo-code

```
while (char = get_next_char()) {
    switch (state) {
        case e_default:
            if (char == quote) {
                if (!current_token || token_finalized) {
                    create_new_token();
                }
                state = e_in_quotes;
            } else if (char == space) {
                finalize_current_token();
            } else {
                append_to_token(char);
            }
            break;
            
        case e_in_quotes:
            if (char == matching_quote) {
                state = e_default;
                // NE PAS finaliser immédiatement!
            } else {
                append_to_token(char);
            }
            break;
    }
}
```