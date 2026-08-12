# Komplex AI Usage Policy (Draft)

| Applies To | Effective Date | Revision Date |
| ------ | --- | --- |
| Internal & Contributors | August 12, 2026 | August 12, 2026 |

This policy may be updated in the future. The updated date will be reflected on this file.

## Applies to
This policy applies to contributors to Komplex and all associated repositories under the Komplex project umbrella.
It outlines how artificial intelligence (AI) tools and systems may be used responsibly in the context of this project.

Our goal is to encourage innovation and productivity while protecting our users, contributors, community, and the integrity of our open source projects. We want to enable employees and contractors to understand what is, and is not, acceptable, and enable the wider community to understand and have trust in how we operate. 

## Scope
This policy applies to any use of AI tools that can materially influence repository outputs, including:
- Generative AI (e.g., image, text and code assistants)
- AI-assisted development tools (e.g., code completion and refactoring assistants)
- Tools that can generate diffs, patches, or substantial edits
- Tools that can draft or summarize issues, bug reports, or diagnostics
- Tools that can generate translations files and reports
- Agenic automation of project tasks

## Guiding principles
1. Human responsibility: Humans remain accountable for decisions, code, and outputs.
2. Transparency: If AI materially contributes to the output, disclose it clearly and concisely in pull requests, commit messages, or accompanying documentation where appropriate.
3. Safety and quality: AI-generated suggestions must be reviewed and validated by humans before being adopted.
4. Integrity and authorship: AI must not be used to bypass authorship, review, or verification requirements.
5. Community and ecosystem trust: AI must not be used to mislead reviewers or circumvent project quality standards.

## Prohibited uses of AI
AI tools may not be used for:
- Agentic generation of code, patches, diffs, or substantial edits intended to become part of the repository.
- Produce a complete file replacement, complete module, or end-to-end implementation of a feature.
- Generate bug report details or where the bug report content would be primarily AI-authored.

## Acceptable uses of AI
AI tools may be used to:
- Assist with writing or refactoring code, provided the resulting code changes are implemented, reviewed and verified by a human contributor.
- Generating support documentation that is based on human-authored doxygen comments.
- Help with debugging by reviewing code, generating hypotheses and proposing targeted experiments to identify root cause in existing code.
- Translate a human-authored bug report into another language, or reformat a human-authored report for clarity.
- Translate and generate application translation files (including strings and localized resources), subject to human review.
- Generate images and icons when used only as placeholders, with the intention to replace them with project-approved assets.

All acceptable uses are subject to change.

### Bug Reports
You must write the factual and investigative aspects of bug reports yourself.
For reporting AI may be used only to:
- Reformat or rewrite for clarity while preserving the original factual content
- Help translate the human-authored report into a different structure, language, or style

### Misrepresentation and undisclosed material contribution
Misrepresentation of AI-assisted work as purely human-authored where disclosure is required by this policy is subject to enforcement policies.

### “Agentic” Automation
You may not use AI tools that can take actions on your behalf without intentional invocation. For example browsing, running commands, editing files and repositories, or opening pull requests automatically.

## Allowed AI uses (how to apply the restrictions)

### Implementation
Allowable AI usage still requires human authorship, review and verification.

- You must implement changes yourself.
- You must verify changes via tests, linting/type checks, and/or manual validation consistent with project standards.
- Do not copy/paste AI-generated patches as the final solution.

### Media placeholders (allowed)
AI-generated images/icons may be used only as placeholders with the intention to replace them.

Placeholder rules:
- Placeholder files must be clearly marked in an AI_ASSETS file.
- Placeholders should be replaced when possible, but may require artist contribution and may not always be immediately feasible.

### Qt Translation Files
AI may be used to generate translation files.

## Transparency and disclosure
When AI materially contributes to work, you must disclose it in one of:

- The pull request description
- Commit messages
- A related comment on the pull request

Disclosure must cover:

- Which allowed category was used (e.g., translation file generation, bug report translation, placeholder media)
- What tool/version was used for generation

## Review and verification requirements
For changes that include AI-assisted suggestions:
- Run the project’s standard checks (tests, linting, formatting, and/or type checks).
- Ensure reviewers can understand the human rationale for changes.
- Ensure the final code and translations meet project quality standards.

## Enforcement
Failure to comply with this policy may result in:

- Requesting changes to bring contributions back into compliance
- Rejection of pull requests
- Removal of contributor privileges where applicable
- Escalation to maintainers for further action

## Policy values statement
AI is a tool, not a substitute for responsibility. In Komplex, AI may be used to increase productivity and assist with debugging, review, translation, and placeholders, but humans must remain accountable for the correctness, safety, and authorship of repository outputs.
